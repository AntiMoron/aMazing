#include "PixelShaderObject.hpp"
using namespace aMazing;

PixelShaderObject::PixelShaderObject()
{
	type = SHADER_PIXEL;
	shader = nullptr;
}


PixelShaderObject::~PixelShaderObject()
{
	if (shader != nullptr)
	{
		shader->Release();
		shader = nullptr;
	}
}

HRESULT PixelShaderObject::createShaderFromFile(ID3D11Device* device,
	ID3D11DeviceContext* context,
	const wchar_t* fileName)
{
	//Create Vertex Shader.If isInited is true,that means the class has already been initialized.
	//return fail.
	return createShaderFromFile(device, context, MutableString(fileName).getMultiByteString().c_str());
}

HRESULT PixelShaderObject::createShaderFromMemory(ID3D11Device* device,
	ID3D11DeviceContext* context,
	const wchar_t* slsource)
{
	return createShaderFromMemory(device, context, MutableString(slsource).getMultiByteString().c_str());
}


HRESULT PixelShaderObject::createShaderFromFile(ID3D11Device* device,
	ID3D11DeviceContext* context,
	const char* fileName)
{
	//Create Vertex Shader.If isInited is true,that means the class has already been initialized.
	//return fail.
	if (bIsInited == true)
	{
		return E_FAIL;
	}
	HRESULT hr;
	bIsInited = false;
	hr = ShaderCompilerClass::compileFromFile(fileName, "PSEntry", "ps_5_0", &pShaderContextBuffer);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	if (pShaderContextBuffer == nullptr)
	{
		return E_FAIL;
	}
	hr = device->CreatePixelShader(pShaderContextBuffer->GetBufferPointer(),
		pShaderContextBuffer->GetBufferSize(),
		nullptr, &shader);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = initializeClassLinkage(device);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = initializeShaderReflector();
	if(FAILED(hr))
	{
		return hr;
	}

	ShaderClassInstance instance(pShaderReflector,"xxx");
	bIsInited = true;
	return S_OK;
}

HRESULT PixelShaderObject::createShaderFromMemory(ID3D11Device* device,
	ID3D11DeviceContext* context,
	const char* slsource)
{
	//Create Vertex Shader.If bIsInited is true,that means the class has already been initialized.
	//return fail.
	if (bIsInited == true)
	{
		return E_FAIL;
	}
	HRESULT hr;
	bIsInited = false;
	hr = ShaderCompilerClass::compileString(slsource, "PSEntry", "ps_5_0", &pShaderContextBuffer);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	if (pShaderContextBuffer == nullptr)
	{
		return E_FAIL;
	}
	hr = device->CreatePixelShader(pShaderContextBuffer->GetBufferPointer(),
		pShaderContextBuffer->GetBufferSize(),
		nullptr, &shader);
	if (FAILED(hr))
	{
		return hr;
	}
	bIsInited = true;
	return S_OK;
}

HRESULT PixelShaderObject::bindShader(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	if (bIsInited == false)
	{
		return E_FAIL;
	}
	context->PSSetShader(shader, dynamicLinkageArray.get(), shaderInterfaceCount);
	return S_OK;
}
