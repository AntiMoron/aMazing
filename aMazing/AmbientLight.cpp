#include "AmbientLight.h"

AmbientLight::AmbientLight()
{
	rasterizeState = nullptr;
}

AmbientLight::~AmbientLight()
{
}

HRESULT AmbientLight::Initialize(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	HRESULT hr;
	matrices.reset(new GPUConstantBuffer<AmbientLightMatrices>);
	hr = matrices->Initialize(device, context, 2);
	if (FAILED(hr))
	{
		return hr;
	}
	target = { .0f, .0f, .0f };
	position = { 0.02f, 0.19f, 0.02f };
	setFov(10.0f);
	near_far = { 0.0001f, 100.0f };
	AmbientLightMatrices cbData = {};
	XMVECTOR st = { position.x, position.y, position.z },
		ed = { target.x, target.y, target.z },
		up = { 0.0f, 1.0f, 0.0f };
	cbData.viewMatrix = XMMatrixLookAtLH(st, ed, up);
	cbData.projectionMatrix = XMMatrixPerspectiveFovLH(fov, ASPECTRATIO, near_far.x, near_far.y);

	cbData.viewMatrix = XMMatrixTranspose(cbData.viewMatrix);
	cbData.projectionMatrix = XMMatrixTranspose(cbData.projectionMatrix);

	matrices->UpdateData(&cbData);
	matrices->UpdateGpu(device, context);
	matrices->BindVertexShader(device, context);


	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.AntialiasedLineEnable = true;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FrontCounterClockwise = false;
	//A fixed bias to reply
//	rasterDesc.DepthBias = -100000;
	//A maximum depth bias allowed
//	rasterDesc.DepthBiasClamp = 0.0f;
	//SlopeScaled Depth Bias
//	rasterDesc.SlopeScaledDepthBias = 1.0f;

//	float bias = (float)rasterDesc.DepthBias * (100000 / (1 << 24)) + rasterDesc.SlopeScaledDepthBias * rasterDesc.DepthBiasClamp;
	device->CreateRasterizerState(&rasterDesc, &rasterizeState);
	context->RSSetState(rasterizeState);
	return S_OK;
}

void AmbientLight::Shutdown()
{
	matrices->Shutdown();
}
void AmbientLight::setFov(const float ang)
{
	fov = ang * 0.017453292f;
}

float AmbientLight::getFov()const
{
	return fov / 0.017453292f;
}

void AmbientLight::setPosition(XMFLOAT3 pos)
{
	position = pos;
}
XMFLOAT3 AmbientLight::getPosition()
{
	return position;
}

void AmbientLight::setTarget(XMFLOAT3 tar)
{
	target = tar;
}
XMFLOAT3 AmbientLight::getTarget()
{
	return target;
}

void AmbientLight::Render(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	AmbientLightMatrices cbData = {};
	XMVECTOR st = { position.x, position.y, position.z },
		ed = { target.x, target.y, target.z },
		up = { 0.0f, 1.0f, 0.0f };
	cbData.viewMatrix = XMMatrixLookAtLH(st, ed, up);
	cbData.projectionMatrix = XMMatrixPerspectiveFovLH(fov, 1.0f, near_far.x, near_far.y);
	cbData.position = { position.x, position.y, position.z, 1.0f };
	cbData.direction = {
		target.x - position.x,
		target.y - position.y,
		target.z - position.z,
		1.0f
	};
	cbData.viewMatrix = XMMatrixTranspose(cbData.viewMatrix);
	cbData.projectionMatrix = XMMatrixTranspose(cbData.projectionMatrix);

	matrices->UpdateData(&cbData);
	matrices->UpdateGpu(device, context);
	matrices->BindVertexShader(device, context);
	matrices->BindPixelShader(device, context);
}