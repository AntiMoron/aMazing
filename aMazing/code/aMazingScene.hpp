#pragma once
#include"engine/effect/EffectClass.hpp"
#include"engine/effect/ShadowMap.hpp"
#include"engine/effect/GlowEffect.hpp"
#include"engine/effect/DepthField.hpp"
#include"MazeGenerator.hpp"
#include"engine/system/SoundClass.hpp"
#include"collision/CollisionWorld.hpp"
#include"DayNightClass.hpp"
#include"WrappedCamera.hpp"
#include"engine/animate/ModelClass.hpp"
#include"engine/shader/shaderclass/cAmbientLight.hpp"
#include"engine/shader/shaderclass/cDirectionalLight.hpp"
#include"engine/shader/shaderclass/cEnvironmentLight.hpp"
#include"engine/shader/shaderclass/cSmoothTexturedMaterial.hpp"

using namespace aMazing;

class aMazingScene
{
public:
	aMazingScene();
	~aMazingScene();

	HRESULT Initialize(HWND hwnd,
		ID3D11Device* device,
		ID3D11DeviceContext* context);
	void Render(ID3D11Device* device,ID3D11DeviceContext* context);
	WrappedCamera* getWrappedCamera();
private:
	std::unique_ptr<WrappedCamera> camera;
	std::unique_ptr<DayNightClass> dayTime;
	std::unique_ptr<GlowEffect> glow;
	std::unique_ptr<ShadowMap> shadow;
	std::unique_ptr<Maze> maze;
	std::unique_ptr<CollisionWorld> collisionWorld;
	std::unique_ptr<ModelClass> model;
	struct ClassInstance
	{
		aMazing::shaderObjects::cAmbientLight ambientLighting;
		aMazing::shaderObjects::cDirectionalLight directLighting;
		aMazing::shaderObjects::cEnvironmentLight environmentLighting;
		aMazing::shaderObjects::cSmoothTexturedMaterial material;
	};
	std::unique_ptr<GPUConstantBuffer<ClassInstance> > classInstanceBuffer;
};
