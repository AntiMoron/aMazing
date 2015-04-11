#include "ModelObject.hpp"
using namespace aMazing;

const aiMatrix4x4 ModelObject::identityMatrix = { 1, 0, 0, 0,
												0, 1, 0, 0,
												0, 0, 1, 0,
												0, 0, 0, 1 };
ModelObject::ModelObject()
{
	is_inited = false;
	render_time = 0.0f;
	isStaticModel = true;
}

bool ModelObject::isStatic() const
{
	return isStaticModel;
}

HRESULT ModelObject::Initialize(ID3D11Device* device,
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
	scene = modelImporter.ReadFile(filename.c_str(),
		ppsteps |
		aiProcess_GenSmoothNormals |
		aiProcess_SplitLargeMeshes |
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded |
		aiProcess_SortByPType |
		0);
	if (!scene)
	{
		//failed on loading asset. 
		//if so get error message & output to console.
		std::cout << modelImporter.GetErrorString() << std::endl;
		return E_FAIL;
	}

	isStaticModel = !(scene->HasAnimations());

	//update the fileLocation
	modelLocation.reset(new std::string(getModelLocation(filename.c_str())));

	if (!isStaticModel)
	{
		sceneAnimator.reset(new SceneAnimator(scene));
		hr = boneTransformations.Initialize(device, context, 3);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	//load the texturess of this model.
	loadTextures(device, context, scene, string.getMultiByteString().c_str());

	//load the meshes of this model.
	loadMeshes(scene);

	if ((animationVertexBuffer.empty() && staticVertexBuffer.empty()) || indices.empty())
	{
		return E_FAIL;
	}
	//send vertices into vertex buffers.
	if (isStaticModel)
	{
		for (int i = 0; i < staticVertices.size(); i++)
		{
			auto& currentBuffer = staticVertexBuffer[i];
			hr = currentBuffer->Initialize(device, context,
				staticVertices[i]->data(),
				staticVertices[i]->size(),
				indices[i]->data(),
				indices[i]->size());
			if (FAILED(hr))
			{
				return hr;
			}
		}
	}
	else
	{
		for (int i = 0; i < animationVertices.size(); i++)
		{
			auto& currentBuffer = animationVertexBuffer[i];
			hr = currentBuffer->Initialize(device, context,
				animationVertices[i]->data(),
				animationVertices[i]->size(),
				indices[i]->data(),
				indices[i]->size());
			if (FAILED(hr))
			{
				return hr;
			}
		}
	}
	return S_OK;
}

void ModelObject::loadTextures(ID3D11Device* device,
	ID3D11DeviceContext* context, 
	const aiScene* pScene,
	const char* modelPath)
{
	if (!(pScene->HasMaterials()))
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
			textures.push_back(std::unique_ptr<TextureClass>(new TextureClass));
			try
			{
				HRESULT hr = textures.back()->LoadFile(device, context, filepath.c_str());
				if (FAILED(hr))
				{
					return ;
				}
			}
			catch (const std::exception& e)
			{
				printf("%s\n",e.what());
				textures.back()->beChessBoard(device, context);
			}
		}
		else
		{
			textures.push_back(std::unique_ptr<TextureClass>(new TextureClass));
			textures.back()->beChessBoard(device, context);
		}
	}
}

void ModelObject::loadMeshes(const aiScene* pScene)
{
	if (!(pScene->HasMeshes()))
	{
		return;
	}

	// load the vertices of meshes.
	std::size_t meshBaseVertexIndex = 0;
	for (int i = 0; i < pScene->mNumMeshes; i++)
	{
		const aiMesh* pMesh = pScene->mMeshes[i];
		if (pMesh == nullptr)
		{
			continue;
		}
		indices.push_back(std::unique_ptr<std::vector<WORD> >(new std::vector<WORD>)); 
		textureIndices.push_back(pMesh->mMaterialIndex);
		if (isStaticModel)
		{
			staticVertices.push_back(std::unique_ptr<std::vector<Vertex> >(new std::vector<Vertex>));
			staticVertexBuffer.push_back(std::unique_ptr<GPUVerticesBuffer<Vertex> >(new GPUVerticesBuffer<Vertex>));
			for (int j = 0; j < pMesh->mNumVertices; j++)
			{
				auto& pos = pMesh->mVertices[j];
				auto& nor = pMesh->mNormals[j];
				auto& tex = pMesh->mTextureCoords[0];
				staticVertices.back()->push_back(Vertex());
				staticVertices.back()->back().position = XMFLOAT3{ pos.x, pos.y, pos.z };
				staticVertices.back()->back().normal = XMFLOAT3{ nor.x, nor.y, nor.z };
				if (tex == nullptr)
				{
					staticVertices.back()->back().texture = XMFLOAT2{ 1.0f, 1.0f };
				}
				else
				{
					staticVertices.back()->back().texture = XMFLOAT2{ tex[j].x, tex[j].y };
				}
			}
		}
		else
		{
			animationVertices.push_back(std::unique_ptr<std::vector<SkinVertex> >(new std::vector<SkinVertex>));
			animationVertexBuffer.push_back(std::unique_ptr<GPUMutableVerticeBuffer<SkinVertex> >(new GPUMutableVerticeBuffer<SkinVertex>));
			std::vector<std::size_t> vertexBoneBindCnt;
			for (int j = 0; j < pMesh->mNumVertices; j++)
			{
				auto& pos = pMesh->mVertices[j];
				auto& nor = pMesh->mNormals[j];
				auto& tex = pMesh->mTextureCoords[0];
				vertexBoneBindCnt.push_back(0);
				animationVertices.back()->push_back(SkinVertex());
				animationVertices.back()->back().position = XMFLOAT4{ pos.x, pos.y, pos.z, 1.0f };
				animationVertices.back()->back().normal = XMFLOAT4{ nor.x, nor.y, nor.z, 1.0f };
				if (tex == nullptr)
				{
					animationVertices.back()->back().texture = XMFLOAT4{ 1.0f, 1.0f, 0.0f, 0.0f };
				}
				else
				{
					animationVertices.back()->back().texture = XMFLOAT4{ tex[j].x, tex[j].y, 0.0f, 0.0f };
				}
			}
		
			for (int k = 0; k < pMesh->mNumBones; k++)
			{
				const auto pBone = pMesh->mBones[k];
				for (int l = 0; l < pBone->mNumWeights; l++)
				{
					const auto weights = pBone->mWeights[l];
					if (weights.mWeight == 0.0f)
					{
						continue; 
					}
					auto& lastMesh = animationVertices.back();
					switch (vertexBoneBindCnt[weights.mVertexId])
					{
					case 0:
						lastMesh->at(weights.mVertexId).setBoneIndex(0, k);
						lastMesh->at(weights.mVertexId).weights.x = weights.mWeight;
						break;
					case 1:
						lastMesh->at(weights.mVertexId).setBoneIndex(1, k);
						lastMesh->at(weights.mVertexId).weights.y = weights.mWeight;
						break;
					case 2:
						lastMesh->at(weights.mVertexId).setBoneIndex(2, k);
						lastMesh->at(weights.mVertexId).weights.z = weights.mWeight;
						break;
					case 3:
						lastMesh->at(weights.mVertexId).setBoneIndex(3, k);
						lastMesh->at(weights.mVertexId).weights.w = weights.mWeight;
						break;
					default:
						printf("out of range\n");
					}
					vertexBoneBindCnt[weights.mVertexId]++;
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
				indices.back()->push_back(pMesh->mFaces[j].mIndices[k]);
			}
		}
		meshBaseVertexIndex += pMesh->mNumVertices;
	}
}

std::string ModelObject::getModelLocation(const char* filename)
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

void ModelObject::bindBonesToGPU(ID3D11Device* device,
	ID3D11DeviceContext* context, 
	aiNode* pNode,
	float animationTime)
{
	const aiMatrix4x4 transform = scene->mRootNode->mTransformation;
	BonesBindData boneCbData = {};
	sceneAnimator->calculate(animationTime, 25);
	auto& boneData = sceneAnimator->getBoneMatrices(pNode);
	for (std::size_t cur = 0; cur < boneData.size(); ++cur)
	{
		boneCbData.boneMatrices[cur] = boneData[cur];
	}
	boneTransformations.UpdateData(&boneCbData);
	boneTransformations.UpdateGpu(device, context);
	boneTransformations.BindVertexShader(device, context);
}

bool ModelObject::isInited()const
{
	return is_inited;
}

void ModelObject::Render(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	BasicObject::UpdatePRS(device, context);

	render_time += 0.01f;
	if (render_time > 20.0f)
	{
		render_time = 0.0f;
	}
	Render(device, context, nullptr);
}

/*
* if pNode is nullptr ,
* then it begin with rootNode
*/
void ModelObject::Render(ID3D11Device* device,
	ID3D11DeviceContext* context,
	aiNode* pNode)
{
	if (pNode == nullptr)
	{
		pNode = scene->mRootNode;
	}
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (int i = 0; i < pNode->mNumMeshes; i++)
	{
		std::size_t meshIndex = pNode->mMeshes[i];
		bindBonesToGPU(device, context, pNode, render_time);
		textures[textureIndices[meshIndex]]->bindPS(device, context, 0);
		if (isStaticModel)
			staticVertexBuffer[meshIndex]->Render(device, context);
		else
			animationVertexBuffer[meshIndex]->Render(device, context);
	}
	
	for (int i = 0; i < pNode->mNumChildren; i++)
	{
		Render(device, context, pNode->mChildren[i]);
	}
}
