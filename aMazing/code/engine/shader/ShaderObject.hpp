#pragma once
#include"../system/MutableString.hpp"
#include"../../common/CommonDef.hpp"
#include"../system/D3DManager.hpp"
#include"ShaderCompilerClass.hpp"
#include"ShaderClassInstance.hpp"
namespace aMazing
{
	class ShaderObject
	{
	public:
		enum SHADER_TYPE
		{
			SHADER_NO_TYPE,
			SHADER_VERTEX,
			SHADER_PIXEL,
		};
	
		ShaderObject()
		{
			bIsInited = false;
			type = SHADER_NO_TYPE;
			pShaderContextBuffer = nullptr;
			pClassLinkage = nullptr;
			pShaderReflector = nullptr;
			shaderInterfaceCount = 0;
			dynamicLinkageArray.reset(nullptr);
		}
		virtual ~ShaderObject()
		{
			SAFE_RELEASE(pShaderContextBuffer);
			SAFE_RELEASE(pClassLinkage);
			SAFE_RELEASE(pShaderReflector);
		}
	
		const SHADER_TYPE getType() const
		{
			return type;
		}
		virtual const bool isInited() const
		{
			return bIsInited;
		}
	protected:
		volatile bool bIsInited;
		volatile size_t shaderInterfaceCount;
		SHADER_TYPE type;
		ID3DBlob* pShaderContextBuffer;
		ID3D11ClassLinkage* pClassLinkage;
		ID3D11ShaderReflection* pShaderReflector;
		//variaties for class instances.
		std::unique_ptr<ID3D11ClassInstance* []> dynamicLinkageArray;

		HRESULT initializeClassLinkage(ID3D11Device* device)
		{
			HRESULT hr = E_FAIL;
			hr = device->CreateClassLinkage(&pClassLinkage);
			if (FAILED(hr) || !pClassLinkage)
			{
				return E_FAIL;
			}
			return S_OK;
		}

		//must called after shader compiled
		HRESULT initializeShaderReflector()
		{
			HRESULT hr = E_FAIL;
			hr = D3DReflect(pShaderContextBuffer->GetBufferPointer(),
				pShaderContextBuffer->GetBufferSize(),
				IID_ID3D11ShaderReflection, 
				reinterpret_cast<void**>(&pShaderReflector));
			if (FAILED(hr))
			{
				return hr;
			}
			return S_OK;
		}

		HRESULT initializeDynamicLinkageArray()
		{
			shaderInterfaceCount = pShaderReflector->GetNumInterfaceSlots();
			dynamicLinkageArray.reset(new ID3D11ClassInstance*[shaderInterfaceCount]);
			for (size_t cur = 0; cur < shaderInterfaceCount; cur++)
			{
				dynamicLinkageArray[cur] = nullptr;
				ID3D11ShaderReflectionVariable* pAmbientLightingVar = pShaderReflector->
					GetVariableByName("g_abstractAmbientLighting");
				//auto type = pAmbientLightingVar->GetBuffer();
				//type->GetVariableByName();
				//auto iAmbientLightingOffset = pAmbientLightingVar->GetInterfaceSlot(cur);

				//dynamicLinkageArray;
			}
			return S_OK;
		}
	private:
		friend class ShaderClassInstance;
	};
}
