#pragma once
#include"D3DClass.h"
#include"CameraClass.h"
#include"EffectClass.h"
#include"ShadowMap.hpp"
#include"GlowEffect.hpp"
#include"DepthField.hpp"
#include"MazeGenerator.h"
#include"SoundClass.h"
#include"CollisionWorld.hpp"
#include"DayNightClass.hpp"
#include"WrappedCamera.h"
class aMazingScene
{
public:
	aMazingScene();
	~aMazingScene();

	HRESULT Initialize(HWND hwnd,
		ID3D11Device* device,
		ID3D11DeviceContext* context);
	void Shutdown();
	void Render(D3DClass* d3dkit);
	WrappedCamera* getCamera();
private:
	std::unique_ptr<WrappedCamera> camera;
	std::unique_ptr<DayNightClass> dayTime;
	std::unique_ptr<GlowEffect> glow;
	std::unique_ptr<Maze> maze;
	std::unique_ptr<SoundClass> sound;
	std::unique_ptr<CollisionWorld> collisionWorld;
};

