#pragma once

#include"../../common/CommonDxSupport.hpp"
#include"../system/ShaderCompilerClass.hpp"

namespace aMazing
{
	class DynamicLinkageHelper
	{
	public:
		DynamicLinkageHelper()
		{
			pClassLinkage = nullptr;
		}
		~DynamicLinkageHelper()
		{
			;
		}
		HRESULT Initialize(ID3D11Device* device,
			ID3D11Device* context,const char* fileName)
		{
			HRESULT hr;
			hr = device->CreateClassLinkage(&pClassLinkage);
			if (FAILED(hr) || pClassLinkage == nullptr)
			{
				return E_FAIL; 
			}

			ID3DBlob* output = nullptr;
			ShaderCompilerClass::compileFromFile(fileName, "PSEntry", "ps_5_0", &output);
			device->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(),
				pPixelShaderBuffer->GetBufferSize(), g_pPSClassLinkage, &g_pPixelShader) );
			return S_OK;
		}
	private:
		ID3D11ClassLinkage* pClassLinkage;
	};
}
