#pragma once
#include"D3DClass.h"
#include"CameraClass.h"
#include"EffectClass.h"
#include"ShadowMap.hpp"
#include"GlowEffect.hpp"
#include"DepthField.hpp"
#include"MazeGenerator.h"
class aMazingScene
{
public:
	aMazingScene();
	~aMazingScene();

	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context);
	void Shutdown();

	void Render(D3DClass* d3dkit,CameraClass* camera);
private:
	std::unique_ptr<ShadowMap> shadowMap;
	std::unique_ptr<GlowEffect> glow;
	std::unique_ptr<DepthField> depthField;
	std::unique_ptr<Maze> maze;
};

