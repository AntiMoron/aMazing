#include "ShaderCompilerClass.h"


ShaderCompilerClass::ShaderCompilerClass()
{
}


ShaderCompilerClass::~ShaderCompilerClass()
{
	;
}


HRESULT ShaderCompilerClass::compileFromFile(MutableString& filename,
	const char* entryPoint,
	const char* shaderTarget,
	ID3DBlob** output)
{
	FileTracker ft;
	ft.LoadFile(filename);
	MutableString stringFromFile = ft.getContext();
	compileString(stringFromFile, entryPoint, shaderTarget, output);
	return S_OK;
}


HRESULT ShaderCompilerClass::compileFromFile(MutableString&& filename,
	const char* entryPoint,
	const char* shaderTarget,
	ID3DBlob** output)
{
	FileTracker ft;
	ft.LoadFile(filename);
	MutableString stringFromFile = ft.getContext();
	compileString(stringFromFile, entryPoint, shaderTarget, output);
	return S_OK;
}

HRESULT ShaderCompilerClass::compileString(MutableString& str
	, const char* entryPoint,
	const char* shaderTarget,
	ID3DBlob** output)
{
	HRESULT hr;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS |
		D3DCOMPILE_OPTIMIZATION_LEVEL3;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* errorMsg = nullptr;
	ID3DBlob* compiled = nullptr;
	std::string shaderstr = str.getMultiByteString();
	hr = D3DCompile(shaderstr.c_str(),
		shaderstr.length(),
		nullptr,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint,
		shaderTarget,
		dwShaderFlags,
		0,
		output,
		&errorMsg);

	if (errorMsg != nullptr)
	{
		char* errorText = new char[errorMsg->GetBufferSize()];
		memcpy(errorText, errorMsg->GetBufferPointer(), errorMsg->GetBufferSize());
		MessageBoxA(NULL, errorText, "Shader Error", MB_OK);
		errorMsg->Release();
		return E_FAIL;
	}
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT ShaderCompilerClass::compileString(MutableString&& str
	, const char* entryPoint,
	const char* shaderTarget,
	ID3DBlob** output)
{
	HRESULT hr;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | 
		D3DCOMPILE_OPTIMIZATION_LEVEL3;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* errorMsg = nullptr;
	ID3DBlob* compiled = nullptr;
	std::string shaderstr = str.getMultiByteString();
	std::cout << shaderstr << std::endl;
	hr = D3DCompile(shaderstr.c_str(),
		shaderstr.length(),
		nullptr,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint,
		shaderTarget,
		dwShaderFlags, 
		0,
		output,
		&errorMsg);
	
	if (errorMsg != nullptr)
	{
		char* errorText = new char[errorMsg->GetBufferSize()];
		memcpy(errorText, errorMsg->GetBufferPointer(), errorMsg->GetBufferSize());
		MessageBoxA(NULL,errorText,"Shader Error",MB_OK);
		errorMsg->Release();
		return E_FAIL;
	}
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}