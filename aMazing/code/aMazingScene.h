#pragma once
#include"engine/system/D3DClass.h"
#include"engine/system/CameraClass.h"
#include"engine/effect/EffectClass.hpp"
#include"engine/effect/ShadowMap.hpp"
#include"engine/effect/GlowEffect.hpp"
#include"engine/effect/DepthField.hpp"
#include"MazeGenerator.h"
#include"engine/system/SoundClass.h"
#include"collision/CollisionWorld.hpp"
#include"DayNightClass.hpp"
#include"WrappedCamera.h"
#include"engine/animate/ModelClass.h"

class aMazingScene
{
public:
	aMazingScene();
	~aMazingScene();

	HRESULT Initialize(HWND hwnd,
		ID3D11Device* device,
		ID3D11DeviceContext* context);
	void Render(D3DClass* d3dkit);
	WrappedCamera* getCamera();
private:
	std::unique_ptr<WrappedCamera> camera;
	std::unique_ptr<DayNightClass> dayTime;
	std::unique_ptr<GlowEffect> glow;
	std::unique_ptr<Maze> maze;
	std::unique_ptr<SoundClass> sound;
	std::unique_ptr<CollisionWorld> collisionWorld;
	std::unique_ptr<ModelClass> model;
};

