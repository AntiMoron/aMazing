#pragma once
#include <D3D11.h>
#include <D3DX11.h>
#include <d3dcompiler.h>
#include <iostream>
#include "../system/FileTracker.h"
#include "MutableString.h"
#define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)
class ShaderCompilerClass
{
public:
	ShaderCompilerClass();
	~ShaderCompilerClass();

	static HRESULT compileFromFile(MutableString& filename,
		const char* entryPoint,
		const char* shaderTarget, 
		ID3DBlob** output);

	static HRESULT compileFromFile(MutableString&& filename,
		const char* entryPoint,
		const char* shaderTarget,
		ID3DBlob** output);

	
	static HRESULT compileString(MutableString& str,
		const char* entryPoint,
		const char* shaderTarget,
		ID3DBlob** output);
	static HRESULT compileString(MutableString&& str,
		const char* entryPoint,
		const char* shaderTarget,
		ID3DBlob** output);

};

