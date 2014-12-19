#pragma once

#include <D3D11.h>
#include <D3DX11.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <map>
#include <array>
#include <unordered_map>
#include <algorithm>
#include "MutableString.h"
#include "BoneClass.hpp"
#include "GPUVerticesBuffer.hpp"
#include "GPUMutableVerticesBuffer.hpp"
#include "Vertex.hpp"
#include "CommonUtil.h"
#include "BasicObject.h"
#include "ShaderManager.h"
#include "TextureClass.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "BonesBindData.hpp"

#pragma comment(lib,"assimp.lib")

class ModelClass : public BasicObject
{
public:
	ModelClass();
	~ModelClass();

	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context,
		MutableString&& string);
	void Shutdown();

	bool ReadModelRecursively(const aiScene* pScene,
		float AnimationTime,
		const aiNode* pNode,
		const aiMatrix4x4& ParentTransform);

	bool isInited()const;

	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context);
private:
	void loadTextures(ID3D11Device* device,
		ID3D11DeviceContext* context,
		const aiScene* pScene,
		const char* modelPath);
	void loadMeshes(const aiScene* pScene);
	void loadBones(const aiScene* pScene);
	
	std::string getModelLocation(const char* filename);
	aiNodeAnim*  findNodeByName(const aiAnimation* pAnim, const aiString& nodeName);
	void calcInterpolatedScaling(aiVector3D* output,float animationTime,const aiNodeAnim* pNodeAnim);
	void calcInterpolatedRotation(aiQuaternion* output, float animationTime, const aiNodeAnim* pNodeAnim);
	void calcInterpolatedTranslation(aiVector3D* output, float animationTime, const aiNodeAnim* pNodeAnim);
	void bindBonesToGPU(ID3D11Device* device,
		ID3D11DeviceContext* context, 
		float animationTime);
	
	bool is_inited;
	std::unique_ptr<Assimp::Importer> importer;
	aiScene* scene;
	//the texture indices
	std::unique_ptr<std::vector<std::size_t> > textureIndices;
	//the queue of textures used
	std::unique_ptr<std::vector<std::unique_ptr<TextureClass> > > textures;
	//the queue of queue according to the queue of meshes.
	std::unique_ptr<std::vector<std::unique_ptr<BoneClass> > > bones;
	//the queue of boneMapping according to the queue of meshes
	std::unique_ptr<std::map<aiString, std::size_t, aiStringLess> > boneMapping;
	//the queue of indices of each mesh.
	std::unique_ptr<std::vector<std::unique_ptr<std::vector<WORD> > > > indices;
	//the queue that saves all the vertices
	std::unique_ptr<std::vector<std::unique_ptr<std::vector<SkinVertex> > > > vertices;
	//the queue of buffer that all the parts from model divided from all the vertices 
	std::unique_ptr<std::vector<std::unique_ptr<GPUMutableVerticeBuffer<SkinVertex> > > > vertexBuffer;
	//the constant buffer that update bones' transformations.
	std::unique_ptr<GPUConstantBuffer<BonesBindData> > boneTransformations;
	//saves the location of model in file systems.
	std::unique_ptr<std::string> modelLocation;
	float render_time;
};