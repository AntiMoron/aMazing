#pragma once
#include "ShaderObject.hpp"
namespace aMazing
{
	class PixelShaderObject : public ShaderObject
	{
	public:
		PixelShaderObject()
		{
			type = SHADER_PIXEL;
			shader = nullptr;
		}
		~PixelShaderObject()
		{
			aSAFE_RELEASE(shader);
		}

		HRESULT createShaderFromFile(ID3D11Device* device,
			const wchar_t* fileName)
		{
			//Create Pixel Shader.If isInited is true,that means the class has already been initialized.
			//return fail.
			return createShaderFromFile(device, MutableString(fileName).getMultiByteString().c_str());
		}

		HRESULT createShaderFromMemory(ID3D11Device* device,
			const wchar_t* slsource)
		{
			return createShaderFromMemory(device, MutableString(slsource).getMultiByteString().c_str());
		}

		HRESULT createShaderFromFile(ID3D11Device* device,
			const char* fileName)
		{
			//Create Pixel Shader.If isInited is true,that means the class has already been initialized.
			//return fail.
			if (bIsInited == true)
			{
				return E_FAIL;
			}
			HRESULT hr;
			bIsInited = false;
			hr = ShaderCompiler::compileFromFile(fileName, "PSEntry", "ps_5_0", &pShaderContextBuffer);
			if (FAILED(hr))
			{
				return E_FAIL;
			}
			if (pShaderContextBuffer == nullptr)
			{
				return E_FAIL;
			}
			hr = device->CreatePixelShader(pShaderContextBuffer->GetBufferPointer(),
				pShaderContextBuffer->GetBufferSize(),
				nullptr, &shader);
			if (FAILED(hr))
			{
				return hr;
			}

			hr = initializeClassLinkage(device);
			if (FAILED(hr))
			{
				return hr;
			}

			hr = initializeComponents();
			if (FAILED(hr))
			{
				return hr;
			}

			//	ShaderClassInstance instance(pShaderReflector,"cAmbientLight");
			bIsInited = true;
			return S_OK;
		}

		HRESULT createShaderFromMemory(ID3D11Device* device,
			const char* slsource)
		{
			//Create Pixel Shader.If bIsInited is true,that means the class has already been initialized.
			//return fail.
			if (bIsInited == true)
			{
				return E_FAIL;
			}
			HRESULT hr;
			bIsInited = false;
			hr = ShaderCompiler::compileString(slsource, "PSEntry", "ps_5_0", &pShaderContextBuffer);
			if (FAILED(hr))
			{
				return E_FAIL;
			}
			if (pShaderContextBuffer == nullptr)
			{
				return E_FAIL;
			}
			hr = device->CreatePixelShader(pShaderContextBuffer->GetBufferPointer(),
				pShaderContextBuffer->GetBufferSize(),
				nullptr, &shader);
			if (FAILED(hr))
			{
				return hr;
			}
			bIsInited = true;
			return S_OK;
		}


		HRESULT bindShader(ID3D11DeviceContext* context) const
		{
			if (bIsInited == false)
			{
				return E_FAIL;
			}
			context->PSSetShader(shader, dynamicLinkageArray.get(), shaderInterfaceCount);
			return S_OK;
		}

	private:
		ID3D11PixelShader* shader;
	};
}
