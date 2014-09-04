#pragma once

#include"GPUConstantBuffer.hpp"
#include<xnamath.h>
#include<D3DX10math.h>
#include"WindowClass.h"

class CameraClass
{
public:
	CameraClass();
	~CameraClass();

	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context);
	void Shutdown();

	void setPosition(const XMFLOAT3& pos);
	void setPosition(const XMFLOAT3&& pos);
	XMFLOAT3 getPosition() const;
	void setRotation(const XMFLOAT3& rot);
	void setRotation(const XMFLOAT3&& rot);
	XMFLOAT3 getRotation() const;
	XMFLOAT3 getUpVector() const;
	void setNearDist(const float val);
	void setFarDist(const float val);
	float getNearDist() const;
	float getFarDist() const;
	void setFov(const float ang);
	float getFov() const;

	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context);
private:
	float fov;
	XMFLOAT2 near_far;
	//near Dist and far Dist
	XMFLOAT3 position; 
	//camera Position
	XMFLOAT3 rotation; 
	// + x conterclockwise,vice versa.
	// + y down,vice versa.
	// + z left,vice versa.
	XMFLOAT3 upVector;
	struct cameraMatrices
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	struct cameraMatrices m_matriceData;
	GPUConstantBuffer<cameraMatrices> m_matrices;
};

