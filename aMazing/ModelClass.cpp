#include "ModelClass.h"


ModelClass::ModelClass()
{
	is_inited = false;
	render_time = 0.0f;
	animIndexCached = false;
	mCurrentAnimIndex = 0;
	ZeroMemory(&identityMatrix,sizeof(identityMatrix));
	identityMatrix.a1 = 1.0f;
	identityMatrix.b2 = 1.0f;
	identityMatrix.c3 = 1.0f;
	identityMatrix.d4 = 1.0f;
}


ModelClass::~ModelClass()
{
	importer->FreeScene();
}

// Retrieves the most recent local transformation matrix for the given node. 
const aiMatrix4x4& ModelClass::GetLocalTransform(const aiNode* node) const
{
	auto it = mNodesByName->find(node);
	if (it == mNodesByName->end())
	{
		return identityMatrix;
	}

	return it->second->localTransform;
}

// ------------------------------------------------------------------------------------------------
// Retrieves the most recent global transformation matrix for the given node. 
const aiMatrix4x4& ModelClass::GetGlobalTransform(const aiNode* node) const
{
	auto it = mNodesByName->find(node);
	if (it == mNodesByName->end())
		return identityMatrix;

	return it->second->globalTransform;
}

void ModelClass::setAnimIndex(std::size_t animIndex)
{
	if (animIndexCached == true && 
		mCurrentAnimIndex == animIndex)
	{
		return ;
	}
	//invalid animation index
	if (animIndex > scene->mNumAnimations)
	{
		return ;
	}
	//cache the animation index
	mCurrentAnimIndex = animIndex;

	rootNode.reset(createNodeTree(scene->mRootNode, nullptr));
}

SceneAnimNode* ModelClass::createNodeTree(aiNode* pNode, SceneAnimNode* parent)
{
	SceneAnimNode* internalNode = new SceneAnimNode(pNode->mName.data);
	internalNode->parent.reset(parent);
	(*mNodesByName)[pNode] = internalNode;
	//copy its transformation
	internalNode->localTransform = pNode->mTransformation;
	return nullptr;
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
		aiProcess_LimitBoneWeights | // limit bone weights to 4 per SkinVertex
		aiProcess_OptimizeMeshes | // join small meshes, if possible;
		aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
		0;

	// 使用导入器导入选定的模型文件 
	importer.reset(new Assimp::Importer);
	scene = (aiScene*)importer->ReadFile(filename.c_str(),
		ppsteps |
		aiProcess_GenSmoothNormals |
		aiProcess_SplitLargeMeshes |
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded |
		aiProcess_SortByPType |
		0);
	if (scene == nullptr)
	{
		//failed on loading asset. 
		//if so get error message & output to console.
		std::cout << importer->GetErrorString() << std::endl;
		return false;
	};
	//update the fileLocation
	modelLocation.reset(new std::string(getModelLocation(filename.c_str())));
	textures.reset(new std::vector<std::unique_ptr<TextureClass> >);
	textureIndices.reset(new std::vector<std::size_t>);

	mNodesByName.reset(new std::map<const aiNode*, SceneAnimNode*>);
	mBoneNodesByName.reset(new std::map<aiString, aiNode*, aiStringLess>);
	//bones.reset(new std::vector<std::unique_ptr<BoneClass> >);
	//boneMapping.reset(new std::map<aiString, std::size_t, aiStringLess>);
	indices.reset(new std::vector<std::unique_ptr<std::vector<WORD> > >);
	vertices.reset(new std::vector<std::unique_ptr<std::vector<SkinVertex> > >);
	vertexBuffer.reset(new std::vector<std::unique_ptr<GPUMutableVerticeBuffer<SkinVertex> > >);
	boneTransformations.reset(new GPUConstantBuffer<BonesBindData>);
	boneTransformations->Initialize(device, context, 3);
	//load the texturess of this model.
	loadTextures(device, context, scene, string.getMultiByteString().c_str());
	//load the bones of this model.
	loadBones(scene);

	//bind bones' data.
	bindBonesToGPU(device, context, 0.0f);

	//load the meshes of this model.
	loadMeshes(scene);
	
	if (vertices->empty() ||
		indices->empty())
	{
		return E_FAIL;
	}
	//send vertices into vertex buffers.
	for (int i = 0; i < vertices->size(); i++)
	{
		hr = vertexBuffer->at(i)->Initialize(device, context, 
			vertices->at(i)->data(), 
			vertices->at(i)->size(),
			indices->at(i)->data(),
			indices->at(i)->size());
		if (FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

void ModelClass::loadTextures(ID3D11Device* device,
	ID3D11DeviceContext* context, 
	const aiScene* pScene,
	const char* modelPath)
{
	if (!pScene->HasMaterials())
	{
		return ;
	}
	//load the materials.
	for (int i = 0; i < pScene->mNumMaterials; i++)
	{
		const aiMaterial* mat = pScene->mMaterials[i];
		int texCount = mat->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE);
		if (texCount > 0)
		{
			aiString filename;
			mat->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &filename);
			std::string filepath = *modelLocation.get();
			filepath += filename.C_Str();
			std::cout << filepath.c_str() << std::endl;
			textures->push_back(std::unique_ptr<TextureClass>(new TextureClass));
			try
			{
				HRESULT hr = textures->back()->LoadFile(device, context, filepath.c_str());
				if (FAILED(hr))
				{
					return ;
				}
			}
			catch (const std::exception& e)
			{
				printf("%s\n",e.what());
				textures->back()->beChessBoard(device, context);
			}
		}
		else
		{
			textures->push_back(std::unique_ptr<TextureClass>(new TextureClass));
			textures->back()->beChessBoard(device, context);
		}
	}
}

void ModelClass::loadMeshes(const aiScene* pScene)
{
	if (!pScene->HasMeshes())
	{
		return;
	}

	//the index and weight for each vertex   pair<boneIndex,weight>
	std::unordered_map<std::size_t, std::vector<std::pair<unsigned char,float> > > vindex2Weight;
	//pre-process bone datas
	/*for (std::size_t cur = 0; cur < bones->size(); ++cur)
	{
		auto& weights = (*bones)[cur]->weights;
		for (std::size_t r = 0; r < weights.size(); ++r)
		{
			std::size_t id = weights[r]->mVertexId;
			if (vindex2Weight.find(id) == vindex2Weight.end())
			{
				vindex2Weight.insert(std::pair<size_t, std::vector<std::pair<unsigned char, float> > >(id, {}));
			}
			vindex2Weight[id].push_back(std::pair<unsigned char,float>(cur,weights[r]->mWeight));
		}
	}*/
	//sort weight data for each vertex.the top4 will be used
	for (auto& entry : vindex2Weight)
	{
		auto& indexAndWeight = entry.second;
		std::sort(indexAndWeight.begin(), indexAndWeight.end(), 
			[](const std::pair<unsigned char, float>& a, const std::pair<unsigned char, float>& b) {
				return a.second > b.second;
		});
	}

	// load the vertices of meshes.
	std::size_t meshBaseVertexIndex = 0;
	for (int i = 0; i < pScene->mNumMeshes; i++)
	{
		const aiMesh* pMesh = pScene->mMeshes[i];
		if (pMesh != nullptr)
		{
			vertices->push_back(std::unique_ptr<std::vector<SkinVertex> >(new std::vector<SkinVertex>));
			indices->push_back(std::unique_ptr<std::vector<WORD> >(new std::vector<WORD>));
			vertexBuffer->push_back(std::unique_ptr<GPUMutableVerticeBuffer<SkinVertex> >(new GPUMutableVerticeBuffer<SkinVertex>));
		}
		else
		{
			continue;
		}
		textureIndices->push_back(pMesh->mMaterialIndex);
		
		for (int j = 0; j < pMesh->mNumVertices; j++)
		{
			auto& pos = pMesh->mVertices[j];
			auto& nor = pMesh->mNormals[j];
			auto& tex = pMesh->mTextureCoords[0];
			vertices->back()->push_back(SkinVertex());
			vertices->back()->back().position = XMFLOAT4{ pos.x, pos.y, pos.z, 1.0f };
			vertices->back()->back().normal = XMFLOAT4{ nor.x, nor.y, nor.z, 1.0f };
			if (tex == nullptr)
			{
				vertices->back()->back().texture = XMFLOAT4{ 1.0f, 1.0f, 0.0f, 0.0f }; 
			}
			else
			{
				vertices->back()->back().texture = XMFLOAT4{ tex[j].x, tex[j].y, 0.0f, 0.0f };
			}
			//get the bone factor.
			auto& vecIndicesWeights = vindex2Weight[vertices->back()->size() - 1 + meshBaseVertexIndex];
			if (vecIndicesWeights.size() > 0)
			{
				vertices->back()->back().boneIndices[0] = vecIndicesWeights[0].first;
				vertices->back()->back().weights.x = vecIndicesWeights[0].second;
				if (vecIndicesWeights.size() > 1)
				{
					vertices->back()->back().boneIndices[1] = vecIndicesWeights[1].first;
					vertices->back()->back().weights.y = vecIndicesWeights[1].second;
					if (vecIndicesWeights.size() > 2)
					{
						vertices->back()->back().boneIndices[2] = vecIndicesWeights[2].first;
						vertices->back()->back().weights.z = vecIndicesWeights[2].second;
						if (vecIndicesWeights.size() > 3)
						{
							vertices->back()->back().boneIndices[3] = vecIndicesWeights[3].first;
						}
					}
				}
			}
		}
		
		for (int j = 0; j < pMesh->mNumFaces; j++)
		{
			if (pMesh->mFaces[j].mNumIndices != 3)
			{
				continue;
			}
			//push back the indices of meshes.
			for (int k = 0; k < pMesh->mFaces[j].mNumIndices; k++)
			{
				indices->back()->push_back(pMesh->mFaces[j].mIndices[k]);
			}
		}
		meshBaseVertexIndex += pMesh->mNumVertices;
	}
}

void ModelClass::loadBones(const aiScene* pScene)
{
	if (false == pScene->HasMeshes())
	{
		return;
	}

	for (std::size_t i = 0; i < pScene->mNumMeshes; ++i)
	{
		const aiMesh* pMesh = pScene->mMeshes[i];
		for (std::size_t n = 0; n < pMesh->mNumBones; ++n)
		{
			const auto bone = pMesh->mBones[n];
			(*mBoneNodesByName)[bone->mName] = pScene->mRootNode->FindNode(bone->mName);
		}
	}

	////get base vertex index of each mesh.
	//std::vector<std::size_t> meshBaseIndices;
	//meshBaseIndices.push_back(0);
	//for (std::size_t cur = 1; cur <pScene->mNumMeshes; ++cur)
	//{
	//	auto lastIndex = meshBaseIndices.back();
	//	meshBaseIndices.push_back(lastIndex + pScene->mMeshes[cur]->mNumVertices);
	//}

	//for (std::size_t i = 0; i < pScene->mNumMeshes; i++)
	//{
	//	const aiMesh* pMesh = pScene->mMeshes[i];
	//	if (false != pMesh->HasBones())
	//	{
	//		for (int j = 0; j < pMesh->mNumBones; j++)
	//		{
	//			const aiBone* pBone= pMesh->mBones[j];
	//			std::cout << pBone->mName.C_Str() << std::endl;
	//			if (boneMapping->find(pBone->mName) == boneMapping->end())
	//			{
	//				boneMapping->insert(std::pair<aiString, std::size_t>(pBone->mName,
	//					bones->size()));
	//				bones->emplace_back(std::unique_ptr<BoneClass>(new BoneClass));
	//				for (int k = 0; k < pBone->mNumWeights; k++)
	//				{
	//					bones->back()->weights.push_back(std::unique_ptr<aiVertexWeight>(new aiVertexWeight(pBone->mWeights[k])));
	//					bones->back()->weights.back()->mVertexId += meshBaseIndices[i];
	//				}
	//				bones->back()->offsetMatrix = pBone->mOffsetMatrix;
	//				//offsetMatrix converts vertex from mesh space to bind space here.We need 'bind space to mesh space'
	//				bones->back()->offsetMatrix.Inverse();
	//			}
	//		}
	//	}
	//}
}

std::string ModelClass::getModelLocation(const char* filename)
{
	std::string result;
	result = filename;
	while (!result.empty() && 
		!(result.back() == '/' ||
		result.back() == '\\'))
	{
		result.pop_back();
	}
	return std::move(result);
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

void ModelClass::bindBonesToGPU(ID3D11Device* device,
	ID3D11DeviceContext* context, 
	float animationTime)
{
	const aiMatrix4x4 transform = scene->mRootNode->mTransformation;
//	ReadModelRecursively(scene, animationTime, scene->mRootNode, transform);
	BonesBindData boneCbData = {};
//	for (std::size_t cur = 0; cur < bones->size(); ++cur)
//	{
//		boneCbData.boneMatrices[cur] = bones->at(cur)->finalTransformation.Transpose();
//		boneCbData.boneMatrices[cur] = bones->at(cur)->finalTransformation;
//	}
	boneTransformations->UpdateData(&boneCbData);
	boneTransformations->UpdateGpu(device, context);
	boneTransformations->BindVertexShader(device, context);
}

bool ModelClass::ReadModelRecursively(const aiScene* pScene, 
	float animationTime,
	const aiNode* pNode,
	const aiMatrix4x4& ParentTransform)
{
	aiString nodeName = pNode->mName;
//	std::cout << nodeName.C_Str() << std::endl;
	for (int i = 0; i < pScene->mNumAnimations; i++)
	{
		const aiAnimation* pAnimation = pScene->mAnimations[i];
		const aiNodeAnim* pNodeAnim = findNodeByName(pAnimation, nodeName);
		//identify the transform matrix
		aiMatrix4x4 nodeTransformation = identityMatrix;

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
			aiMatrix4x4 temp;
			nodeTransformation.Scaling(scaling, temp);
			nodeTransformation *= temp;
			nodeTransformation.RotationX(rotation.x, temp);
			nodeTransformation *= temp;
			nodeTransformation.RotationY(rotation.y, temp);
			nodeTransformation *= temp;
			nodeTransformation.RotationZ(rotation.z, temp);
			nodeTransformation *= temp;
			nodeTransformation.Translation(position, temp);
			nodeTransformation *= temp;
			/*printf("%f %f %f %f\n"
				"%f %f %f %f\n"
				"%f %f %f %f\n"
				"%f %f %f %f\n\n", nodeTransformation.a1, nodeTransformation.a2, nodeTransformation.a3, nodeTransformation.a4,
				nodeTransformation.b1, nodeTransformation.b2, nodeTransformation.b3, nodeTransformation.b4, 
				nodeTransformation.c1, nodeTransformation.c2, nodeTransformation.c3, nodeTransformation.c4, 
				nodeTransformation.d1, nodeTransformation.d2, nodeTransformation.d3, nodeTransformation.d4);*/
		}
		//get the toRoot matrix
		aiMatrix4x4 globalTransform = ParentTransform;
//		globalTransform.Inverse();
		aiMatrix4x4 currentTransformation = globalTransform * nodeTransformation;
		if (boneMapping->find(nodeName) != boneMapping->end())
		{
			std::size_t BoneIndex = boneMapping->operator[](nodeName);
			bones->at(BoneIndex)->finalTransformation = currentTransformation * bones->operator[](BoneIndex)->offsetMatrix;
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

	render_time += 0.003f;
	if(render_time > 1.0f)
	{
		render_time = 0.0f;
	}

	bindBonesToGPU(device,context,render_time);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (int i = 0; i < vertexBuffer->size(); i++)
	{
		if (vertexBuffer->at(i)->isInited())
		{
			textures->at(textureIndices->at(i))->bindPS(device, context, 0);
			vertexBuffer->at(i)->Render(device, context);
		}
	}
}