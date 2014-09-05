#include "ShaderPair.h"


ShaderPair::ShaderPair()
{
}


ShaderPair::~ShaderPair()
{
	Shutdown();
}

ShaderPair::ShaderPair(VertexShaderClass** pv,
	PixelShaderClass** pp)
{
	pVert = *pv;
	pPixl = *pp;
}


HRESULT ShaderPair::Shutdown()
{
	if (pVert != nullptr)
	{
		pVert->Shutdown();
		delete pVert;
		pVert = nullptr;
	}
	if (pPixl != nullptr)
	{
		pPixl->Shutdown();
		delete pPixl;
		pPixl = nullptr;
	}
	return S_OK;
}

HRESULT ShaderPair::bindShader(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	HRESULT hr;
	hr = pVert->bindShader(device,context);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = pPixl->bindShader(device, context);
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}