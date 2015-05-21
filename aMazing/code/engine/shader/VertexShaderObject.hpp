#pragma once
#include"ShaderCompiler.hpp"
#include "ShaderObject.hpp"
namespace aMazing
{
	class VertexShaderObject : public ShaderObject
	{
	public:
		VertexShaderObject()
		{
			type = SHADER_VERTEX;
			shader = nullptr;
			layout = nullptr;
		}
		~VertexShaderObject()
		{
			aSAFE_RELEASE(shader);
			aSAFE_RELEASE(layout);
		}

		template<size_t numElements>
		HRESULT createShaderFromFile(ID3D11Device* device,
			const char* fileName,
			D3D11_INPUT_ELEMENT_DESC(&layoutDesc)[numElements])
		{
			//Create Vertex Shader.If isInited is true,that means the class has already been initialized.
			//return fail.
			if (bIsInited == true)
			{
				return E_FAIL;
			}
			HRESULT hr;
			bIsInited = false;
			hr = ShaderCompiler::compileFromFile(fileName, "VSEntry", "vs_5_0", &pShaderContextBuffer);
			if (FAILED(hr))
			{
				return E_FAIL;
			}
			if (pShaderContextBuffer == nullptr)
			{
				return E_FAIL;
			}
			hr = device->CreateVertexShader(pShaderContextBuffer->GetBufferPointer(),
				pShaderContextBuffer->GetBufferSize(),
				nullptr, &shader);
			if (FAILED(hr))
			{
				return hr;
			}
			// Create the input layout
			hr = device->CreateInputLayout(layoutDesc, numElements, pShaderContextBuffer->GetBufferPointer(),
				pShaderContextBuffer->GetBufferSize(), &layout);
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
			bIsInited = true;
			return S_OK;
		}

		template<size_t numElements>
		HRESULT createShaderFromMemory(ID3D11Device* device,
			const char* slsource,
			D3D11_INPUT_ELEMENT_DESC(&layoutDesc)[numElements])
		{
			//Create Vertex Shader.If isInited is true,that means the class has already been initialized.
			//return fail.
			if (bIsInited == true)
			{
				return E_FAIL;
			}
			HRESULT hr;
			bIsInited = false;
			hr = ShaderCompiler::compileString(slsource, "VSEntry", "vs_5_0", &pShaderContextBuffer);
			if (FAILED(hr))
			{
				return E_FAIL;
			}
			if (pShaderContextBuffer == nullptr)
			{
				return E_FAIL;
			}
			hr = device->CreateVertexShader(pShaderContextBuffer->GetBufferPointer(),
				pShaderContextBuffer->GetBufferSize(),
				nullptr, &shader);
			if (FAILED(hr))
			{
				return hr;
			}
			// Create the input layout
			hr = device->CreateInputLayout(layoutDesc, numElements,
				pShaderContextBuffer->GetBufferPointer(),
				pShaderContextBuffer->GetBufferSize(), &layout);

			if (FAILED(hr))
			{
				return hr;
			}
			bIsInited = true;
			return S_OK;
		}

		template<size_t numElements>
		HRESULT createShaderFromFile(ID3D11Device* device,
			const wchar_t* fileName,
			D3D11_INPUT_ELEMENT_DESC (&layoutDesc)[numElements])
		{
			return createShaderFromFile(device,
				MutableString(fileName).getMultiByteString().c_str(),
				layoutDesc);
		}

		template<size_t numElements>
		HRESULT createShaderFromMemory(ID3D11Device* device,
			const wchar_t* slsource,
			D3D11_INPUT_ELEMENT_DESC (&layoutDesc)[numElements])
		{
			return createShaderFromMemory(device,
				MutableString(slsource).getMultiByteString().c_str(),
				layoutDesc);
		}

		HRESULT bindShader(ID3D11DeviceContext* context) const
		{
			if (bIsInited == false)
			{
				return E_FAIL;
			}
			context->VSSetShader(shader, /*dynamicLinkageArray.get()*/nullptr, shaderInterfaceCount);
			context->IASetInputLayout(layout);
			return S_OK;
		}

	private:
		ID3D11VertexShader* shader;
		ID3D11InputLayout* layout;
	};
}
