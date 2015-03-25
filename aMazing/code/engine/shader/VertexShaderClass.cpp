#include "VertexShaderClass.hpp"
using namespace aMazing;

VertexShaderClass::VertexShaderClass()
{
	type = SHADER_VERTEX;
	shader = nullptr;
	layout = nullptr;
}


VertexShaderClass::~VertexShaderClass()
{
	SAFE_RELEASE(shader);
	SAFE_RELEASE(layout);
}

HRESULT VertexShaderClass::createShaderFromFile(ID3D11Device* device,
	ID3D11DeviceContext* context,
	const char* fileName,
	D3D11_INPUT_ELEMENT_DESC layoutDesc[],
	unsigned int numElements)
{
	//Create Vertex Shader.If isInited is true,that means the class has already been initialized.
	//return fail.
	if (isInited == true)
	{
		return E_FAIL;
	}
	HRESULT hr;
	isInited = false;
	hr = ShaderCompilerClass::compileFromFile(fileName, "VSEntry", "vs_5_0", &pShaderContextBuffer);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	if (pShaderContextBuffer == nullptr)
	{
		return E_FAIL;
	}
	hr = device->CreateVertexShader(pShaderContextBuffer->GetBufferPointer(),
		pShaderContextBuffer->GetBufferSize(),
		nullptr, &shader);
	if (FAILED(hr))
	{
		return hr;
	}
	// Create the input layout
	hr = device->CreateInputLayout(layoutDesc, numElements, pShaderContextBuffer->GetBufferPointer(),
		pShaderContextBuffer->GetBufferSize(), &layout);

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
	if (FAILED(hr))
	{
		return hr;
	}

	isInited = true;
	return S_OK;
}

HRESULT VertexShaderClass::createShaderFromMemory(ID3D11Device* device,
	ID3D11DeviceContext* context,
	const char* slsource,
	D3D11_INPUT_ELEMENT_DESC layoutDesc[],
	unsigned int numElements)
{
	//Create Vertex Shader.If isInited is true,that means the class has already been initialized.
	//return fail.
	if (isInited == true)
	{
		return E_FAIL;
	}
	HRESULT hr;
	isInited = false;
	hr = ShaderCompilerClass::compileString(slsource, "VSEntry", "vs_5_0", &pShaderContextBuffer);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	if (pShaderContextBuffer == nullptr)
	{
		return E_FAIL;
	}
	hr = device->CreateVertexShader(pShaderContextBuffer->GetBufferPointer(),
		pShaderContextBuffer->GetBufferSize(),
		nullptr, &shader);
	if (FAILED(hr))
	{
		return hr;
	}
	// Create the input layout
	hr = device->CreateInputLayout(layoutDesc, numElements, 
		pShaderContextBuffer->GetBufferPointer(),
		pShaderContextBuffer->GetBufferSize(), &layout);

	if (FAILED(hr))
	{
		return hr;
	}
	isInited = true;
	return S_OK;
}


HRESULT VertexShaderClass::createShaderFromFile(ID3D11Device* device,
	ID3D11DeviceContext* context,
	const wchar_t* fileName,
	D3D11_INPUT_ELEMENT_DESC layoutDesc[],
	unsigned int numElements)
{
	return createShaderFromFile(device,context,
		MutableString(fileName).getMultiByteString().c_str(),
		layoutDesc,
		numElements);
}

HRESULT VertexShaderClass::createShaderFromMemory(ID3D11Device* device,
	ID3D11DeviceContext* context,
	const wchar_t* slsource,
	D3D11_INPUT_ELEMENT_DESC layoutDesc[],
	unsigned int numElements)
{
	return createShaderFromMemory(device,context,
		MutableString(slsource).getMultiByteString().c_str(),
		layoutDesc,
		numElements);
}

HRESULT VertexShaderClass::bindShader(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	if (isInited == false)
	{
		return E_FAIL;
	}
	context->VSSetShader(shader, dynamicLinkageArray.get(), shaderInterfaceCount);
	context->IASetInputLayout(layout);
	return S_OK;
}
