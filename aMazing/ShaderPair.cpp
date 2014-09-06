#include "ShaderPair.h"

ShaderPair::~ShaderPair()
{
	Shutdown();
}

ShaderPair::ShaderPair(VertexShaderClass** pv,
	PixelShaderClass** pp,
	std::string&& shadername)
{
	pVert = *pv;
	pPixl = *pp;
	shaderName = shadername;
}

bool ShaderPair::operator < (const ShaderPair& other)const
{
	return shaderName < other.shaderName;
}

bool ShaderPair::operator < (const std::string& other)const
{
	return shaderName < other;
}

bool ShaderPair::operator == (const std::string& other)const
{
	return shaderName == other;
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
	if (pVert == nullptr || pPixl == nullptr)
	{
		return E_FAIL;
	}
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