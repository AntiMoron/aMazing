#include "ModelClass.h"


ModelClass::ModelClass()
{
	is_inited = false;
}


ModelClass::~ModelClass()
{
}

HRESULT ModelClass::Initialize(ID3D11Device* device,
	ID3D11DeviceContext* context,
	MutableString&& string)
{
	HRESULT hr = E_FAIL;
	hr = BasicObject::Initialize(device, context);
	if (FAILED(hr))
	{
		return hr;
	}
	
	Assimp::Importer importer;

	std::string filename = string.getMultiByteString(); 

	unsigned int ppsteps = aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
		aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
		aiProcess_ValidateDataStructure | // perform a full validation of the loader's output
		aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials | // remove redundant materials
		aiProcess_FindDegenerates | // remove degenerated polygons from the import
		aiProcess_FindInvalidData | // detect invalid model data, such as invalid nor vectors
		aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
		aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
		aiProcess_LimitBoneWeights | // limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes | // join small meshes, if possible;
		aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
		0;

	// 使用导入器导入选定的模型文件 
	const aiScene* scene = importer.ReadFile(filename.c_str(),
		ppsteps | /* configurable pp steps */
		aiProcess_GenSmoothNormals | // generate smooth nor vectors if not existing
		aiProcess_SplitLargeMeshes | // split large, unrenderable meshes into submeshes
		aiProcess_Triangulate | // triangulate polygons with more than 3 edges
		aiProcess_ConvertToLeftHanded | // convert everything to D3D left handed space
		aiProcess_SortByPType | // make 'clean' meshes which consist of a single typ of primitives
		0);                //后处理标志，将不同图元放置到不同的模型中去，图片类型可能是点、直线、三角形等
	if (!scene)
	{
		//导入错误，获取错误信息并进行相应的处理   
		std::cout << importer.GetErrorString() << std::endl;
		return false;
	};
	const aiMatrix4x4 transform =  scene->mRootNode->mTransformation;
	vertices.reset(new std::vector<vertex>);
	indices.reset(new std::vector<WORD>);
	bones.reset(new std::vector<std::unique_ptr<BoneClass> >);
	boneMapping.reset(new std::map<aiString, std::size_t, aiStringLess>);
	vertexBuffer.reset(new GPUMutableVerticeBuffer<vertex>);
	//load the meshes of this model.
	loadMeshes(scene);
	//load the bones of this model.
	loadBones(scene);

	ReadModelRecursively(scene, 0.0f, scene->mRootNode, transform);

	if (vertices->empty() ||
		indices->empty())
	{
		return E_FAIL;
	}

	hr = vertexBuffer->Initialize(device, context, 
		vertices->data(), 
		vertices->size(),
		indices->data(),
		indices->size());
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

void ModelClass::Shutdown()
{
	BasicObject::Shutdown();
	if (vertexBuffer.get() != nullptr)
	{
		vertexBuffer->Shutdown();
	}
}

void ModelClass::loadMeshes(const aiScene* pScene)
{
	if (!pScene->HasMeshes())
	{
		return;
	}
	// load the vertices of meshes.
	for (int i = 0; i < pScene->mNumMeshes; i++)
	{
		const aiMesh* pMesh = pScene->mMeshes[i];
		for (int j = 0; j < pMesh->mNumVertices; j++)
		{
			auto& pos = pMesh->mVertices[j];
			auto& nor = pMesh->mNormals[j];
			auto& tex = pMesh->mTextureCoords;
			vertices->push_back(vertex());
			vertices->back().position = XMFLOAT3{ pos.x, pos.y, pos.z };
			vertices->back().normal = XMFLOAT3{ nor.x, nor.y, nor.z };
			//Problem left.!!!!!!!!!!!!
			vertices->back().texture = XMFLOAT2{ .0f , .0f};
		}

		for (int j = 0; j < pMesh->mNumFaces; j++)
		{
			if (pMesh->mFaces[j].mNumIndices != 3)
			{
				continue;
			}
			//push back the indices of meshes.
			indices->push_back(pMesh->mFaces[j].mIndices[0]);
			indices->push_back(pMesh->mFaces[j].mIndices[1]);
			indices->push_back(pMesh->mFaces[j].mIndices[2]);
		}
	}
}

void ModelClass::loadBones(const aiScene* pScene)
{
	if (false == pScene->HasMeshes())
	{
		return;
	}
	for (int i = 0; i < pScene->mNumMeshes; i++)
	{
		const aiMesh* pMesh = pScene->mMeshes[i];
		if (false == pMesh->HasBones())
		{
			for (int j = 0; j < pMesh->mNumBones; j++)
			{
				const aiBone* pBone= pMesh->mBones[j];
				std::cout << pBone->mName.C_Str() << std::endl;
				if (boneMapping->find(pBone->mName) == boneMapping->end())
				{
					boneMapping->insert(std::pair<aiString, std::size_t>(pBone->mName,
						bones->size()));
					bones->emplace_back(std::unique_ptr<BoneClass>(new BoneClass));
					for (int k = 0; k < pBone->mNumWeights; k++)
					{
						bones->back()->weights.push_back(
							std::unique_ptr<aiVertexWeight>(new aiVertexWeight(pBone->mWeights[k])));
					}
					bones->back()->boneOffSet = pBone->mOffsetMatrix;
					bones->back()->finalTransformation = bones->back()->boneOffSet;
				}
			}
		}
	}
}

aiNodeAnim* ModelClass::findNodeByName(const aiAnimation* pAnim, const aiString& nodeName)
{
	for (int i = 0; i < pAnim->mNumChannels; i++)
	{
		aiString& current = pAnim->mChannels[i]->mNodeName;
		if (current == nodeName)
		{
			return pAnim->mChannels[i];
		}
	}
	return nullptr;
}


void ModelClass::calcInterpolatedScaling(aiVector3D* output, float animationTime, const aiNodeAnim* pNodeAnim)
{
	*output = { .0f, .0f, .0f };
	//compare with the last frame of this frame queue.If current time is larger than the last frame.
	//then return its value directly.
	if (pNodeAnim->mNumScalingKeys > 0 &&
		animationTime >= pNodeAnim->mScalingKeys[pNodeAnim->mNumScalingKeys - 1].mTime)
	{
		*output = pNodeAnim->mScalingKeys[pNodeAnim->mNumScalingKeys - 1].mValue;
		return;
	}

	for (int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
	{
		const auto frameKey = pNodeAnim->mScalingKeys[i];
		const auto nextFrameKey = pNodeAnim->mScalingKeys[i + 1];
		if (animationTime >= frameKey.mTime)
		{
			double timeInterval = animationTime - frameKey.mTime;
			double timeRange = nextFrameKey.mTime - frameKey.mTime;
			aiVector3D range = nextFrameKey.mValue - frameKey.mValue;
			range /= timeRange;
			range *= timeInterval;
			*output = range;
			*output += frameKey.mValue;
			break;
		}
	}
}

void ModelClass::calcInterpolatedRotation(aiQuaternion* output, float animationTime, const aiNodeAnim* pNodeAnim)
{
	*output = { .0f, .0f, .0f };
	//compare with the last frame of this frame queue.If current time is larger than the last frame.
	//then return its value directly.
	if (pNodeAnim->mNumRotationKeys > 0 &&
		animationTime >= pNodeAnim->mRotationKeys[pNodeAnim->mNumRotationKeys - 1].mTime)
	{
		*output = pNodeAnim->mRotationKeys[pNodeAnim->mNumRotationKeys - 1].mValue;
		return;
	}

	for (int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
	{
		const auto frameKey = pNodeAnim->mRotationKeys[i];
		const auto nextFrameKey = pNodeAnim->mRotationKeys[i + 1];
		if (animationTime >= frameKey.mTime)
		{
			double timeInterval = animationTime - frameKey.mTime;
			double timeRange = nextFrameKey.mTime - frameKey.mTime;
			aiQuaternion range = nextFrameKey.mValue;
			range.x -= frameKey.mValue.x;
			range.y -= frameKey.mValue.y;
			range.z -= frameKey.mValue.z;
			range.w -= frameKey.mValue.w;
			output->x = range.x / timeRange * timeInterval + frameKey.mValue.x;
			output->y = range.y / timeRange * timeInterval + frameKey.mValue.y;
			output->z = range.z / timeRange * timeInterval + frameKey.mValue.z;
			output->w = range.w / timeRange * timeInterval + frameKey.mValue.w;
			break;
		}
	}
}

void ModelClass::calcInterpolatedTranslation(aiVector3D* output, float animationTime, const aiNodeAnim* pNodeAnim)
{
	*output = { .0f, .0f, .0f };
	//compare with the last frame of this frame queue.If current time is larger than the last frame.
	//then return its value directly.
	if (pNodeAnim->mNumPositionKeys > 0 &&
		animationTime >= pNodeAnim->mPositionKeys[pNodeAnim->mNumPositionKeys - 1].mTime)
	{
		*output = pNodeAnim->mPositionKeys[pNodeAnim->mNumPositionKeys - 1].mValue;
		return;
	}

	for (int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
	{
		const auto frameKey = pNodeAnim->mPositionKeys[i];
		const auto nextFrameKey = pNodeAnim->mPositionKeys[i + 1];
		if (animationTime >= frameKey.mTime)
		{
			double timeInterval = animationTime - frameKey.mTime;
			double timeRange = nextFrameKey.mTime - frameKey.mTime;
			aiVector3D range = nextFrameKey.mValue - frameKey.mValue;
			range /= timeRange;
			range *= timeInterval;
			*output = range;
			*output += frameKey.mValue;
			break;
		}
	}
}

bool ModelClass::ReadModelRecursively(const aiScene* pScene, 
	float animationTime,
	const aiNode* pNode,
	const aiMatrix4x4& ParentTransform)
{
	aiString nodeName = pNode->mName;
	std::cout << nodeName.C_Str() << std::endl;
	for (int i = 0; i < pScene->mNumAnimations; i++)
	{
		const aiAnimation* pAnimation = pScene->mAnimations[i];
		const aiNodeAnim* pNodeAnim = findNodeByName(pAnimation, nodeName);
		//identify the transform matrix
		aiMatrix4x4 nodeTransformation;
		ZeroMemory(&nodeTransformation,sizeof(nodeTransformation));
		nodeTransformation.a1 = 1.0f;
		nodeTransformation.b2 = 1.0f;
		nodeTransformation.c3 = 1.0f;
		nodeTransformation.d4 = 1.0f;

		//if can't find the corresponding NodeAnim of this node then continue;
		if (pNodeAnim != nullptr)
		{
			//get the scaling matrix
			aiVector3D scaling = { .0f, .0f, .0f };
			calcInterpolatedScaling(&scaling, animationTime, pNodeAnim);
			//get the rotation matrix
			aiQuaternion rotation = { .0f, .0f, .0f, .0f };
			calcInterpolatedRotation(&rotation, animationTime, pNodeAnim);
			//get the translation matrix
			aiVector3D position = { .0f, .0f, .0f };
			calcInterpolatedTranslation(&position, animationTime, pNodeAnim);

			nodeTransformation.Decompose(scaling, rotation, position);
		}

		aiMatrix4x4 currentTransformation = ParentTransform * nodeTransformation;
		if (boneMapping->find(nodeName) != boneMapping->end())
		{
			std::size_t BoneIndex = boneMapping->operator[](nodeName);
			bones->operator[](BoneIndex)->finalTransformation = nodeTransformation *
				bones->operator[](BoneIndex)->boneOffSet;
		}

		//do the recursion
		for (int i = 0; i < pNode->mNumChildren; i++) 
		{
			ReadModelRecursively(pScene,
				animationTime,
				pNode->mChildren[i], 
				currentTransformation);
		}
	}
	return true;
}


bool ModelClass::isInited()const
{
	return is_inited;
}


void ModelClass::Render(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	BasicObject::UpdatePRS(device, context);
	if (vertexBuffer->isInited())
	{
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		vertexBuffer->Render(device, context);
	}
}