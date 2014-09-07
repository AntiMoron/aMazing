#include "VertexShaderClass.h"


VertexShaderClass::VertexShaderClass()
{
	type = SHADER_VERTEX;
	isInited = false;
	shader = nullptr;
	layout = nullptr;
}


VertexShaderClass::~VertexShaderClass()
{
	Shutdown();
}

void VertexShaderClass::Shutdown()
{
	if (shader != nullptr)
	{
		shader->Release();
		shader = nullptr;
	}
	if (layout != nullptr)
	{
		layout->Release();
		layout = nullptr;
	}
}

HRESULT VertexShaderClass::createShaderFromFile(ID3D11Device* device,
	ID3D11DeviceContext* context,
	MutableString & filename,
	D3D11_INPUT_ELEMENT_DESC layoutdesc[],
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
	ID3DBlob* output = nullptr;
	hr = ShaderCompilerClass::compileFromFile(filename, "VSEntry", "vs_5_0", &output);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	if (output == nullptr)
	{
		return E_FAIL;
	}
	hr = device->CreateVertexShader(output->GetBufferPointer(),
		output->GetBufferSize(),
		nullptr, &shader);
	if (FAILED(hr))
	{
		return hr;
	}
	// Create the input layout
	hr = device->CreateInputLayout(layoutdesc, numElements, output->GetBufferPointer(),
		output->GetBufferSize(), &layout);
	output->Release();

	if (FAILED(hr))
	{
		return hr;
	}
	isInited = true;
	return S_OK;
}

HRESULT VertexShaderClass::createShaderFromMemory(ID3D11Device* device,
	ID3D11DeviceContext* context,
	MutableString & slsource,
	D3D11_INPUT_ELEMENT_DESC layoutdesc[],
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
	ID3DBlob* output = nullptr;
	hr = ShaderCompilerClass::compileString(slsource, "VSEntry", "vs_5_0", &output);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	if (output == nullptr)
	{
		return E_FAIL;
	}
	hr = device->CreateVertexShader(output->GetBufferPointer(),
		output->GetBufferSize(),
		nullptr, &shader);
	if (FAILED(hr))
	{
		return hr;
	}
	// Create the input layout
	hr = device->CreateInputLayout(layoutdesc, numElements, output->GetBufferPointer(),
		output->GetBufferSize(), &layout);
	output->Release();

	if (FAILED(hr))
	{
		return hr;
	}
	isInited = true;
	return S_OK;
}


HRESULT VertexShaderClass::createShaderFromFile(ID3D11Device* device,
	ID3D11DeviceContext* context,
	MutableString && filename,
	D3D11_INPUT_ELEMENT_DESC layoutdesc[], 
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
	ID3DBlob* output = nullptr;
	hr = ShaderCompilerClass::compileFromFile(filename,"VSEntry","vs_5_0",&output);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	if (output == nullptr)
	{
		return E_FAIL;
	}
	hr = device->CreateVertexShader(output->GetBufferPointer(),
		output->GetBufferSize(),
		nullptr,&shader);
	if (FAILED(hr))
	{
		return hr;
	}

	// Create the input layout
	hr = device->CreateInputLayout(layoutdesc, numElements, output->GetBufferPointer(),
		output->GetBufferSize(), &layout);
	output->Release();

	if (FAILED(hr))
		return hr;
	isInited = true;
	return S_OK;
}

HRESULT VertexShaderClass::createShaderFromMemory(ID3D11Device* device,
	ID3D11DeviceContext* context,
	MutableString && slsource,
	D3D11_INPUT_ELEMENT_DESC layoutdesc[],
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
	ID3DBlob* output = nullptr;
	hr = ShaderCompilerClass::compileString(slsource, "VSEntry", "vs_5_0", &output);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	if (output == nullptr)
	{
		return E_FAIL;
	}
	hr = device->CreateVertexShader(output->GetBufferPointer(),
		output->GetBufferSize(),
		nullptr, &shader);
	if (FAILED(hr))
	{
		return hr;
	}
	// Create the input layout
	hr = device->CreateInputLayout(layoutdesc, numElements, output->GetBufferPointer(),
		output->GetBufferSize(), &layout);
	output->Release();

	if (FAILED(hr))
	{
		return hr;
	}

	isInited = true;
	return S_OK;
}

HRESULT VertexShaderClass::bindShader(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	if (isInited == false)
	{
		return E_FAIL;
	}
	context->VSSetShader(shader, nullptr, 0);
	context->IASetInputLayout(layout);
	return S_OK;
}