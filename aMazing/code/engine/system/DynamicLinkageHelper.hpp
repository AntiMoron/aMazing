#pragma once

#include"../../common/CommonDxSupport.hpp"
#include"../shader/ShaderCompilerClass.hpp"
#include<memory>
namespace aMazing
{
	class DynamicLinkageHelper
	{
	public:
		DynamicLinkageHelper()
		{
			pClassLinkage = nullptr;
			pShaderRelector = nullptr;
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

			ID3DBlob* pixelShader = nullptr;
			ShaderCompilerClass::compileFromFile(fileName, "PSEntry", "ps_5_0", &pixelShader);
			device->CreatePixelShader(pixelShader->GetBufferPointer(),
				pixelShader->GetBufferSize(), g_pPSClassLinkage, &g_pPixelShader) );
			D3DReflect(pixelShader->GetBufferPointer(),
				pixelShader->GetBufferSize(),
				IID_ID3D11ShaderReflection, (void**)&pReflector) );
			const int g_iNumPSInterfaces = pReflector->GetNumInterfaceSlots();

			std::unique_ptr<ID3D11ClassInstance*[]> dynamicLinkageArray(new ID3D11ClassInstance*[g_iNumPSInterfaces]);

			ID3D11ShaderReflectionVariable* pAmbientLightingVar =
				pReflector->GetVariableByName("g_abstractAmbientLighting");
			g_iAmbientLightingOffset = pAmbientLightingVar->GetInterfaceSlot(0);

			return S_OK;
		}

	private:
		ID3D11ClassLinkage* pClassLinkage;
		ID3D11ShaderReflection* pShaderRelector;
	};
}
