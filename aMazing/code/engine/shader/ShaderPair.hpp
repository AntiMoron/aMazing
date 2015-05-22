#pragma once
#include"ShaderLiteral.hpp"
#include"VertexShaderObject.hpp"
#include"PixelShaderObject.hpp"
#include<memory>
namespace aMazing
{
	class ShaderPair
	{
	private:
		template<size_t cnt>
		friend std::shared_ptr<ShaderPair> aMakeShaderPairFromMemory(ID3D11Device* device,
			const char* vsContent,
			const char* psContent,
			D3D11_INPUT_ELEMENT_DESC(&layoutDesc)[cnt],
			const std::string& name);
		template<size_t cnt>
		friend std::shared_ptr<ShaderPair> aMakeShaderPairFromFile(ID3D11Device* device,
			const char* vsFilePath,
			const char* psFilePath,
			D3D11_INPUT_ELEMENT_DESC(&layoutDesc)[cnt],
			const std::string& name);
	public:
		ShaderPair(VertexShaderObject** pv,
			PixelShaderObject** pp)
		{
			pVert = *pv;
			pPixl = *pp;
		}
		~ShaderPair()
		{
			if (pVert != nullptr)
			{
				delete pVert;
				pVert = nullptr;
			}
			if (pPixl != nullptr)
			{
				delete pPixl;
				pPixl = nullptr;
			}
		}


		HRESULT bindShader(ID3D11DeviceContext* context) const
		{
			HRESULT hr;
			if (pVert == nullptr || pPixl == nullptr)
			{
				return E_FAIL;
			}
			hr = pVert->bindShader(context);
			if (FAILED(hr))
			{
				return hr;
			}
			hr = pPixl->bindShader(context);
			if (FAILED(hr))
			{
				return hr;
			}
			return S_OK;
		}

		bool isValid() const
		{
			return !((pVert == nullptr) || (pPixl == nullptr));
		}
	private:
		std::string shaderName;
		VertexShaderObject* pVert;
		PixelShaderObject*  pPixl;
	};

	template<size_t cnt>
	std::shared_ptr<ShaderPair> aMakeShaderPairFromMemory(ID3D11Device* device,
		const char* vsContent,
		const char* psContent,
		const D3D11_INPUT_ELEMENT_DESC (&layoutDesc)[cnt])
	{
		HRESULT hr = E_FAIL;
		VertexShaderObject* v = new VertexShaderObject;
		hr = v->createShaderFromMemory(device, vsContent, layoutDesc);
		if (FAILED(hr))
		{
			aDBG("Error in content: " << vsContent);
			aSAFE_DELETE(v);
		}
		PixelShaderObject* p = new PixelShaderObject;
		hr = p->createShaderFromMemory(device, psContent);
		if (FAILED(hr))
		{
			aDBG("Error in content: " << psContent);
			aSAFE_DELETE(p);
		}
		return std::make_shared<ShaderPair>(&v, &p);
	}

	template<size_t cnt>
	std::shared_ptr<ShaderPair> aMakeShaderPairFromFile(ID3D11Device* device,
		const char* vsFilePath,
		const char* psFilePath,
		const D3D11_INPUT_ELEMENT_DESC(&layoutDesc)[cnt])
	{
		HRESULT hr = E_FAIL;
		VertexShaderObject* v = new VertexShaderObject;
		hr = v->createShaderFromFile(device, vsFilePath, layoutDesc);
		if (FAILED(hr))
		{
			aDBG("Error At: " << vsFilePath);
			aSAFE_DELETE(v);
		}
		PixelShaderObject* p = new PixelShaderObject;
		hr = p->createShaderFromFile(device, psFilePath);
		if (FAILED(hr))
		{
			aDBG("Error At: " << vsFilePath);
			aSAFE_DELETE(p);
		}
		return std::make_shared<ShaderPair>(&v, &p);
	}
}
