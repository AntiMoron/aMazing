#include "AmbientLight.h"

AmbientLight::AmbientLight()
{
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
	position = { -0.1f, -0.1f, -0.1f };
	setFov(40.0f);
	near_far = { 0.001f, 1000.0f };
	AmbientLightMatrices cbData = {};
	XMVECTOR st = { position.x, position.y, position.z },
		ed = { target.x, target.y, target.z },
		up = { 0.0f, 1.0f, 0.0f };
	cbData.viewMatrix = XMMatrixLookAtLH(st, ed, up);
	cbData.projectionMatrix = XMMatrixPerspectiveFovLH(fov, ASPECTRATIO, near_far.x, near_far.y);;

	cbData.viewMatrix = XMMatrixTranspose(cbData.viewMatrix);
	cbData.projectionMatrix = XMMatrixTranspose(cbData.projectionMatrix);

	matrices->UpdateData(&cbData);
	matrices->UpdateGpu(device, context);
	matrices->BindVertexShader(device, context);
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
	cbData.projectionMatrix = XMMatrixPerspectiveFovLH(fov, ASPECTRATIO, near_far.x, near_far.y);;

	cbData.viewMatrix = XMMatrixTranspose(cbData.viewMatrix);
	cbData.projectionMatrix = XMMatrixTranspose(cbData.projectionMatrix);

	matrices->UpdateData(&cbData);
	matrices->UpdateGpu(device, context);
	matrices->BindVertexShader(device, context);
}