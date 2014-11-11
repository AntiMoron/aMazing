#pragma once

#include <D3D11.h>
#include <D3DX11.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <map>
#include <unordered_map>
#include "MutableString.h"
#include "BoneClass.hpp"
#include "GPUVerticesBuffer.hpp"
#include "GPUMutableVerticesBuffer.hpp"
#include "vertex.hpp"
#include "CommonUtil.h"
#include "BasicObject.h"
#include "ShaderManager.h"
#include "TextureClass.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define MAX_BONE 550

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
	
	bool is_inited;
	const aiScene* scene;
	std::unique_ptr<std::vector<std::size_t> > textureIndices;
	std::unique_ptr<std::vector<std::unique_ptr<TextureClass> > > textures;
	std::unique_ptr<std::vector<std::unique_ptr<BoneClass> > > bones;
	std::unique_ptr<std::map<aiString, std::size_t, aiStringLess> > boneMapping;
	std::unique_ptr<std::vector<std::unique_ptr<std::vector<WORD> > > > indices;
	std::unique_ptr<std::vector<std::unique_ptr<std::vector<vertex> > > > vertices;
	std::unique_ptr<std::vector<std::unique_ptr<GPUMutableVerticeBuffer<vertex> > > > vertexBuffer;

	std::unique_ptr<std::string> modelLocation;
	float render_time;
};