#include "ShaderPair.hpp"
using namespace aMazing;

ShaderPair::~ShaderPair()
{
	if (pVert != nullptr)
	{
		delete pVert;
		pVert = nullptr;
	}
	if (pPixl != nullptr)
	{
		delete pPixl;
		pPixl = nullptr;
	}
}

ShaderPair::ShaderPair(VertexShaderObject** pv,
	PixelShaderObject** pp,
	std::string&& shadername)
{
	pVert = *pv;
	pPixl = *pp;
	shaderName = shadername;
}

ShaderPair::ShaderPair(const ShaderPair& other)
{
	pPixl = other.pPixl;
	pVert = other.pVert;
	shaderName = other.shaderName;
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

HRESULT ShaderPair::bindShader(ID3D11DeviceContext* context)
{
	HRESULT hr;
	if (pVert == nullptr || pPixl == nullptr)
	{
		return E_FAIL;
	}
	hr = pVert->bindShader(context);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = pPixl->bindShader(context);
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
