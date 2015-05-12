#pragma once
#include"../system/MutableString.hpp"
#include"../../common/CommonDef.hpp"
#include"../system/D3DManager.hpp"
#include"ShaderCompiler.hpp"
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
			aSAFE_RELEASE(pShaderContextBuffer);
			aSAFE_RELEASE(pClassLinkage);
			aSAFE_RELEASE(pShaderReflector);
		}
	
		const SHADER_TYPE getType() const
		{
			return type;
		}
		virtual const bool isInited() const
		{
			return bIsInited;
		}
		//To get the object of class inherited from interface.
		//If failed to get the object,or didn't find the name in hlsl,
		//returns null pointer.Otherwise return the object
		ID3D11ClassInstance* getInterfaceObjectByName(const char* objectName)
		{
			HRESULT hr = E_FAIL;
			auto shaderVar = pShaderReflector->GetVariableByName(objectName);
			unsigned int interfaceSlot = shaderVar->GetInterfaceSlot(0);
			if (signed(interfaceSlot) < 0)
			{
				return nullptr;
			}
			ID3D11ClassInstance* classInstance = nullptr;
			hr = pClassLinkage->GetClassInstance(objectName, interfaceSlot, &classInstance);
			if (FAILED(hr))
			{
				return nullptr;
			}
			if (!classInstance)
			{
				return nullptr;
			}
			dynamicLinkageArray[interfaceSlot] = classInstance;			
			return dynamicLinkageArray[interfaceSlot];
		}

		//To get the element of sepcified index of array object of class
		//inherited from interface in hlsl.
		//If failed to get the object,or didn't find the name in hlsl,
		//returns null pointer.Otherwise return the object
		ID3D11ClassInstance* getInterfaceArrayElementByName(const char* arrayName, size_t index)
		{
			HRESULT hr = E_FAIL;
			auto shaderVar = pShaderReflector->GetVariableByName(arrayName);
			unsigned int interfaceSlot = shaderVar->GetInterfaceSlot(index);
			if (signed(interfaceSlot) < 0)
			{
				return nullptr;
			}
			ID3D11ClassInstance* classInstance = nullptr;
			hr = pClassLinkage->GetClassInstance(arrayName, interfaceSlot, &classInstance);
			if (FAILED(hr))
			{
				return nullptr;
			}
			if (!classInstance)
			{
				return nullptr;
			}
			dynamicLinkageArray[interfaceSlot] = classInstance;
			return dynamicLinkageArray[interfaceSlot];
		}

		ID3D11ClassInstance* createClassInstance(const char* className)
		{
			ID3D11ClassInstance* result = nullptr;
			pClassLinkage->CreateClassInstance(className, 0, 0, 0, 0, &result);
			return result;
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
			if (shaderInterfaceCount == 0)
			{
				dynamicLinkageArray.reset(nullptr);
			}
			else
			{
				dynamicLinkageArray.reset(new ID3D11ClassInstance*[shaderInterfaceCount]);
				for (size_t i = 0; i < shaderInterfaceCount ;i++)
				{
					dynamicLinkageArray[i] = nullptr;
				}
			}
			return S_OK;
		}

		HRESULT initializeComponents()
		{
			HRESULT hr = E_FAIL;
			hr = initializeShaderReflector();
			if (FAILED(hr))
			{
				return hr;
			}
			hr = initializeDynamicLinkageArray();
			if (FAILED(hr))
			{
				return hr;
			}
			return S_OK;
		}
	private:
		friend class ShaderClassInstance;
	};
}
