#pragma once

#include <string>
#include <vector>
#include "../system/GPUVerticesBuffer.hpp"
#include "../system/GPUMutableVerticesBuffer.hpp"
#include "../data/Vertex.hpp"
#include "../../common/CommonUtil.hpp"
#include "../system/BasicObject.hpp"
#include "../shader/ShaderManager.hpp"
#include "../system/TextureObject.hpp"
#include "../data/BonesBindData.hpp"
#include "SceneAnimator.hpp"

#pragma comment(lib,"assimp.lib")
namespace aMazing
{
	class ModelObject : public BasicObject
	{
	private:
		Assimp::Importer modelImporter;
		const static aiMatrix4x4 identityMatrix;
	public:
		ModelObject();

		HRESULT initialize(ID3D11Device* device,
			MutableString&& string);

		bool isInited()const;


		void render(ID3D11DeviceContext* context);

		bool isStatic() const;
	private:

		void render(ID3D11DeviceContext* context,
			aiNode* pNode);

		//load all the textures from resource files.
		void loadTextures(ID3D11Device* device,
			const aiScene* pScene,
			const char* modelPath);
		//load mesh data from asset.
		void loadMeshes(const aiScene* pScene);
		//load bones' data from asset.
		void loadBones(const aiScene* pScene);
		//return the string of the path where the model is located.	
		std::string getModelLocation(const char* filename);
	
		//bind bones from CPU to GPU.
		void bindBonesToGPU(ID3D11DeviceContext* context, 
			aiNode* pNode,
			float animationTime);

		volatile bool is_inited;
		volatile bool isStaticModel;

		//the data loaded from file.
		const aiScene* scene;

		/******************texture data**********************/
		//the texture indices
		std::vector<std::size_t> textureIndices;
		//the queue of textures used
		std::vector<std::unique_ptr<TextureObject> > textures;

		/**********************vertex data.**********************/
		//the queue of indices of each mesh.
		std::vector<std::unique_ptr<std::vector<WORD> > > indices;
		//the queue that saves all the vertices
		std::vector<std::unique_ptr<std::vector<SkinVertex> > > animationVertices;
		std::vector<std::unique_ptr<std::vector<Vertex> > > staticVertices;
		//the queue of buffer that all the parts from model divided from all the vertices 
		std::vector<std::unique_ptr<GPUMutableVerticeBuffer<SkinVertex> > > animationVertexBuffer;
		std::vector<std::unique_ptr<GPUVerticesBuffer<Vertex> > > staticVertexBuffer;

		std::unique_ptr<SceneAnimator> sceneAnimator;
		//the constant buffer that update bones' transformations.
		GPUConstantBuffer<BonesBindData> boneTransformations;
		//saves the location of model in file systems.
		std::unique_ptr<std::string> modelLocation;
		float render_time;
};
}