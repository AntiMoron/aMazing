#pragma once
#include"../../common/CommonDxSupport.hpp"
#include<memory>
#include"WindowClass.hpp"
#include"GPUConstantBuffer.hpp"
#include"../../common/CommonUtil.hpp"

class AmbientLight
{
public:
	AmbientLight();
	~AmbientLight();

	HRESULT Initialize(ID3D11Device* device, 
		ID3D11DeviceContext* context);

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
		XMFLOAT4 direction;
		XMFLOAT4 position;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};
	float fov;
	XMFLOAT2 near_far;
	std::unique_ptr<GPUConstantBuffer<AmbientLightMatrices> > matrices;
	ID3D11RasterizerState* rasterizeState;
};

