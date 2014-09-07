#include "EffectClass.h"


EffectClass::EffectClass()
{
	is_init = false;
}


EffectClass::~EffectClass()
{
}


HRESULT EffectClass::Initialize(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	HRESULT hr;
	hr = fbo.Initialize(device, context);
	if (FAILED(hr))
	{
		return hr;
	}
	is_init = true;
	return S_OK;
}

HRESULT EffectClass::Shutdown()
{
	fbo.Shutdown();
	return S_OK;
}

void EffectClass::setRenderTarget(ID3D11Device* device,
	ID3D11DeviceContext* context,
	ID3D11DepthStencilView* depth)
{
	fbo.setRenderTarget(device, context, depth);
	fbo.clearRenderTarget(device, context, depth);
}

void EffectClass::bindVS(ID3D11Device* device,
	ID3D11DeviceContext* context,
	unsigned int textureSlot)
{
	fbo.bindVS(device, context, textureSlot);
}

void EffectClass::bindPS(ID3D11Device* device,
	ID3D11DeviceContext* context,
	unsigned int textureSlot)
{
	fbo.bindPS(device, context, textureSlot);
}

bool EffectClass::isInited()const
{
	return is_init;
}