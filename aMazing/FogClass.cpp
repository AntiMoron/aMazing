#include "FogClass.h"


FogClass::FogClass()
{
}


FogClass::~FogClass()
{
}


HRESULT FogClass::Initialize(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	HRESULT hr;
	fogBuffer.reset(new GPUConstantBuffer<FOG_DESC>);
	hr = fogBuffer->Initialize(device, context, 5);
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

void FogClass::setFogColor(XMFLOAT3 color)
{
	fogDesc.fogColor = color;
}

void FogClass::setFogNearDist(float val_near)
{
	fogDesc.nearDist = val_near;
}

void FogClass::setFogFarDist(float val_far)
{
	fogDesc.farDist = val_far;
}

void FogClass::Render(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	FOG_DESC cbData = fogDesc;
	fogBuffer->UpdateData(&cbData);
	fogBuffer->UpdateGpu(device, context);
	fogBuffer->BindVertexShader(device,context);
	fogBuffer->BindPixelShader(device, context);
}