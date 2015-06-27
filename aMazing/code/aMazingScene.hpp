#pragma once
#include"engine/effect/EffectClass.hpp"
#include"engine/system/D3DManager.hpp"
#include"MazeGenerator.hpp"
#include"engine/system/SoundClass.hpp"
#include"collision/CollisionWorld.hpp"
#include"DayNightClass.hpp"
#include"WrappedCamera.hpp"
#include"engine/asset/ModelObject.hpp"
#include"engine/shader/shaderclass/cAmbientLight.hpp"
#include"engine/shader/shaderclass/cDirectionalLight.hpp"
#include"engine/shader/shaderclass/cEnvironmentLight.hpp"
#include"engine/shader/shaderclass/cSmoothTexturedMaterial.hpp"
using namespace aMazing;

class aMazingScene
{
public:
	aMazingScene();

	HRESULT initialize(HWND hwnd,
		ID3D11Device* device);
	void render(ID3D11DeviceContext* context);
	const std::shared_ptr<WrappedCamera>& getWrappedCamera();
private:
	std::shared_ptr<ModelObject> model;
	std::shared_ptr<WrappedCamera> camera;
	std::shared_ptr<DayNightClass> dayTime;
	std::shared_ptr<Maze> maze;
	std::shared_ptr<CollisionWorld> collisionWorld;
	std::shared_ptr<FrameBuffer> fb;
	struct ClassInstance
	{
		aMazing::shaderObjects::cAmbientLight ambientLighting;
		aMazing::shaderObjects::cDirectionalLight directLighting;
		aMazing::shaderObjects::cEnvironmentLight environmentLighting;
		aMazing::shaderObjects::cSmoothTexturedMaterial material;
	};
	std::unique_ptr<GPUConstantBuffer<ClassInstance> > classInstanceBuffer;
};
