#include "ShaderCompilerClass.hpp"
using namespace aMazing;

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
	return compileFromFile(std::forward<MutableString&&>(filename), entryPoint, shaderTarget, output);
	//FileTracker ft;
	//if (false == ft.LoadFile(filename))
	//{
	//	return E_FAIL;
	//}
	//MutableString stringFromFile = ft.getContext();
	//HRESULT hr;
	//hr = compileString(stringFromFile, entryPoint, shaderTarget, output);
	//if (FAILED(hr))
	//{
	//	return hr;
	//}
	//if (output == nullptr)
	//{
	//	return E_FAIL;
	//}
	//return S_OK;
}


HRESULT ShaderCompilerClass::compileFromFile(MutableString&& filename,
	const char* entryPoint,
	const char* shaderTarget,
	ID3DBlob** output)
{
	bool  resetCurrentDir = false;
	wchar_t workingPath[MAX_PATH];
	auto nBytes = GetCurrentDirectoryW(MAX_PATH, workingPath);
	if (nBytes >= MAX_PATH)
	{
		return E_FAIL;
	}
	FileTracker ft;
	if (ft.LoadFile(filename) == false)
	{
		return E_FAIL;
	}
	// Check if the file is in the current working directory
	auto wFileName = filename.getWideString();
	auto lastSplashInd = wFileName.rfind(TEXT('\\'));
	auto lastSplashInd2 = wFileName.rfind(TEXT('/'));
	lastSplashInd = aMin<decltype(lastSplashInd)>(lastSplashInd, lastSplashInd2);
	if (lastSplashInd != std::wstring::npos)
	{
		SetCurrentDirectoryW(wFileName.substr(0,lastSplashInd).c_str());
		resetCurrentDir = true;
	}
	MutableString stringFromFile = ft.getContext();
	compileString(stringFromFile, entryPoint, shaderTarget, output);
	if (resetCurrentDir)
	{
		SetCurrentDirectory(workingPath);
	}
	return S_OK;
}

HRESULT ShaderCompilerClass::compileString(MutableString& str
	, const char* entryPoint,
	const char* shaderTarget,
	ID3DBlob** output)
{
	return compileString(std::forward<MutableString&&>(str),
		entryPoint,
		shaderTarget,
		output);
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
