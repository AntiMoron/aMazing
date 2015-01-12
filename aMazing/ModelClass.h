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
#include "SceneAnimNode.hpp"

#pragma comment(lib,"assimp.lib")

class ModelClass : public BasicObject
{
public:
	ModelClass();
	~ModelClass();

	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context,
		MutableString&& string);

	bool ReadModelRecursively(const aiScene* pScene,
		float AnimationTime,
		const aiNode* pNode,
		const aiMatrix4x4& ParentTransform);

	bool isInited()const;

	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context);
private:
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
	aiNodeAnim*  findNodeByName(const aiAnimation* pAnim, const aiString& nodeName);
	void calcInterpolatedScaling(aiVector3D* output,float animationTime,const aiNodeAnim* pNodeAnim);
	void calcInterpolatedRotation(aiQuaternion* output, float animationTime, const aiNodeAnim* pNodeAnim);
	void calcInterpolatedTranslation(aiVector3D* output, float animationTime, const aiNodeAnim* pNodeAnim);
	//bind bones from CPU to GPU.
	void bindBonesToGPU(ID3D11Device* device,
		ID3D11DeviceContext* context, 
		float animationTime);

	// Retrieves the most recent local transformation matrix for the given node. 
	const aiMatrix4x4& GetLocalTransform(const aiNode* node) const;
	// Retrieves the most recent global transformation matrix for the given node. 
	const aiMatrix4x4& GetGlobalTransform(const aiNode* node) const;

	//Set the animation to use for playback.
	void setAnimIndex(std::size_t animIndex);
	//create animation tree.
	SceneAnimNode* createNodeTree(aiNode* pNode,SceneAnimNode* parent);
	
	volatile bool is_inited;
	std::unique_ptr<Assimp::Importer> importer;
	//the data loaded from file.
	aiScene* scene;

	/******************texture data**********************/
	//the texture indices
	std::unique_ptr<std::vector<std::size_t> > textureIndices;
	//the queue of textures used
	std::unique_ptr<std::vector<std::unique_ptr<TextureClass> > > textures;

	/**********************vertex data.**********************/
	//the queue of indices of each mesh.
	std::unique_ptr<std::vector<std::unique_ptr<std::vector<WORD> > > > indices;
	//the queue that saves all the vertices
	std::unique_ptr<std::vector<std::unique_ptr<std::vector<SkinVertex> > > > vertices;
	//the queue of buffer that all the parts from model divided from all the vertices 
	std::unique_ptr<std::vector<std::unique_ptr<GPUMutableVerticeBuffer<SkinVertex> > > > vertexBuffer;

	/**********************skeleton animation data.**********************/
	//the queue of queue according to the queue of meshes.
	std::unique_ptr<std::vector<std::unique_ptr<BoneClass> > > bones;
	//the queue of boneMapping according to the queue of meshes
	std::unique_ptr<std::map<aiString, std::size_t, aiStringLess> > boneMapping;

	aiMatrix4x4 identityMatrix;

	bool animIndexCached;
	std::size_t mCurrentAnimIndex;
	std::unique_ptr<SceneAnimNode> rootNode;
	std::unique_ptr<std::map<const aiNode*, SceneAnimNode*> > mNodesByName;
	std::unique_ptr<std::map<aiString, aiNode*, aiStringLess> > mBoneNodesByName;
	//the constant buffer that update bones' transformations.
	std::unique_ptr<GPUConstantBuffer<BonesBindData> > boneTransformations;
	//saves the location of model in file systems.
	std::unique_ptr<std::string> modelLocation;
	float render_time;
};