#pragma once
#include"VertexShaderObject.hpp"
#include"PixelShaderObject.hpp"
#include<memory>
namespace aMazing
{
	class ShaderPair
	{
	public:
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

	
		ShaderPair(VertexShaderObject** pv,
			PixelShaderObject** pp,const std::string& shaderName)
		{
			pVert = *pv;
			pPixl = *pp;
			this->shaderName = shaderName;
		}


		ShaderPair(const ShaderPair& other)
		{
			pPixl = other.pPixl;
			pVert = other.pVert;
			this->shaderName = other.shaderName;
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


		bool operator < (const ShaderPair& other)const
		{
			return shaderName < other.shaderName;
		}

		bool operator < (const std::string& other)const
		{
			return shaderName < other;
		}

		bool operator == (const std::string& other)const
		{
			return shaderName == other;
		}


	private:
		std::string shaderName;
		VertexShaderObject* pVert;
		PixelShaderObject*  pPixl;
	};
}
