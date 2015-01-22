#pragma once

#include"CommonDxSupport.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <map>
#include <array>
#include <unordered_map>
#include <algorithm>
#include "BoneClass.hpp"
#include "GPUVerticesBuffer.hpp"
#include "GPUMutableVerticesBuffer.hpp"
#include "Vertex.hpp"
#include "CommonUtil.hpp"
#include "BasicObject.h"
#include "ShaderManager.h"
#include "TextureClass.h"
#include "BonesBindData.hpp"
#include "SceneAnimator.hpp"

#pragma comment(lib,"assimp.lib")

class ModelClass : public BasicObject
{
public:
	ModelClass();
	~ModelClass();

	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context,
		MutableString&& string);
/*
	bool ReadModelRecursively(const aiScene* pScene,
		float AnimationTime,
		const aiNode* pNode,
		const aiMatrix4x4& ParentTransform);*/

	bool isInited()const;


	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context);
private:

	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		aiNode* pNode);

	//load all the textures from resource files.
	void loadTextures(ID3D11Device* device,
		ID3D11DeviceContext* context,
		const aiScene* pScene,
		const char* modelPath);
	//load mesh data from asset.
	void loadMeshes(const aiScene* pScene);
	//load bones' data from asset.
	void loadBones(const aiScene* pScene);
	//return the string of the path where the model is located.	
	std::string getModelLocation(const char* filename);
	
	//bind bones from CPU to GPU.
	void bindBonesToGPU(ID3D11Device* device,
		ID3D11DeviceContext* context, 
		aiNode* pNode,
		float animationTime);

	volatile bool is_inited;
	std::unique_ptr<Assimp::Importer> importer;

	//the data loaded from file.
	aiScene* scene;

	/******************texture data**********************/
	//the texture indices
	std::vector<std::size_t> textureIndices;
	//the queue of textures used
	std::vector<std::unique_ptr<TextureClass> > textures;

	/**********************vertex data.**********************/
	//the queue of indices of each mesh.
	std::vector<std::unique_ptr<std::vector<WORD> > > indices;
	//the queue that saves all the vertices
	std::vector<std::unique_ptr<std::vector<SkinVertex> > > vertices;
	//the queue of buffer that all the parts from model divided from all the vertices 
	std::vector<std::unique_ptr<GPUMutableVerticeBuffer<SkinVertex> > > vertexBuffer;
	
	aiMatrix4x4 identityMatrix;
	std::unique_ptr<SceneAnimator> sceneAnimator;
	//the constant buffer that update bones' transformations.
	GPUConstantBuffer<BonesBindData> boneTransformations;
	//saves the location of model in file systems.
	std::unique_ptr<std::string> modelLocation;
	float render_time;
};