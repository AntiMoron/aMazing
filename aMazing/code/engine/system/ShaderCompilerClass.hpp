#pragma once
#include <D3D11.h>
#include <D3DX11.h>
#include <d3dcompiler.h>
#include <iostream>
#include <utility>
#include <iterator>
#include "../../common/CommonTemplates.hpp"
#include "../system/FileTracker.hpp"
#include "MutableString.hpp"
#define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)

namespace aMazing
{
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
}
