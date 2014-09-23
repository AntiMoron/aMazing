#pragma once
#include<D3D11.h>
#include<D3DX11.h>
#include<xnamath.h>
#include<memory>
#include"WindowClass.h"
#include"GPUConstantBuffer.hpp"

class AmbientLight
{
public:
	AmbientLight();
	~AmbientLight();

	HRESULT Initialize(ID3D11Device* device, 
		ID3D11DeviceContext* context);
	void Shutdown();

	void setFov(const float ang);
	float getFov()const;

	void setPosition(XMFLOAT3 pos);
	XMFLOAT3 getPosition();

	void setTarget(XMFLOAT3 tar);
	XMFLOAT3 getTarget();

	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context);
private:
	XMFLOAT3 position;
	XMFLOAT3 target;

	struct AmbientLightMatrices
	{
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};
	float fov;
	XMFLOAT2 near_far;
	std::unique_ptr<GPUConstantBuffer<AmbientLightMatrices> > matrices;
};

