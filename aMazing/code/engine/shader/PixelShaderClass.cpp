#include "PixelShaderClass.hpp"
using namespace aMazing;

PixelShaderClass::PixelShaderClass()
{
	type = SHADER_PIXEL;
	isInited = false;
	shader = nullptr;
}


PixelShaderClass::~PixelShaderClass()
{
	if (shader != nullptr)
	{
		shader->Release();
		shader = nullptr;
	}
}

HRESULT PixelShaderClass::createShaderFromFile(ID3D11Device* device,
	ID3D11DeviceContext* context,
	const wchar_t* fileName)
{
	//Create Vertex Shader.If isInited is true,that means the class has already been initialized.
	//return fail.
	return createShaderFromFile(device, context, MutableString(fileName).getMultiByteString().c_str());
}

HRESULT PixelShaderClass::createShaderFromMemory(ID3D11Device* device,
	ID3D11DeviceContext* context,
	const wchar_t* slsource)
{
	return createShaderFromMemory(device, context, MutableString(slsource).getMultiByteString().c_str());
}


HRESULT PixelShaderClass::createShaderFromFile(ID3D11Device* device,
	ID3D11DeviceContext* context,
	const char* fileName)
{
	//Create Vertex Shader.If isInited is true,that means the class has already been initialized.
	//return fail.
	if (isInited == true)
	{
		return E_FAIL;
	}
	HRESULT hr;
	isInited = false;
	ID3DBlob* output = nullptr;
	hr = ShaderCompilerClass::compileFromFile(fileName, "PSEntry", "ps_5_0", &output);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	if (output == nullptr)
	{
		return E_FAIL;
	}
	hr = device->CreatePixelShader(output->GetBufferPointer(),
		output->GetBufferSize(),
		nullptr, &shader);
	if (FAILED(hr))
	{
		return hr;
	}
	isInited = true;
	return S_OK;
}

HRESULT PixelShaderClass::createShaderFromMemory(ID3D11Device* device,
	ID3D11DeviceContext* context,
	const char* slsource)
{
	//Create Vertex Shader.If isInited is true,that means the class has already been initialized.
	//return fail.
	if (isInited == true)
	{
		return E_FAIL;
	}
	HRESULT hr;
	isInited = false;
	ID3DBlob* output = nullptr;
	hr = ShaderCompilerClass::compileString(slsource, "PSEntry", "ps_5_0", &output);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	if (output == nullptr)
	{
		return E_FAIL;
	}
	hr = device->CreatePixelShader(output->GetBufferPointer(),
		output->GetBufferSize(),
		nullptr, &shader);
	if (FAILED(hr))
	{
		return hr;
	}
	isInited = true;
	return S_OK;
}

HRESULT PixelShaderClass::bindShader(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	if (isInited == false)
	{
		return E_FAIL;
	}
	context->PSSetShader(shader, nullptr, 0);
	return S_OK;
}
