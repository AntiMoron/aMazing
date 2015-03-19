#pragma once
#include<D3D11.h>
#include<D3DX11.h>
#include"ShaderCompilerClass.hpp"
#include "ShaderClass.hpp"
namespace aMazing
{
	class VertexShaderClass : public ShaderClass
	{
	public:
		VertexShaderClass();
		~VertexShaderClass();

		HRESULT createShaderFromFile(ID3D11Device* device,
			ID3D11DeviceContext* context,
			const char* filename,
			D3D11_INPUT_ELEMENT_DESC layoutdesc[],
			unsigned int numElements);
		HRESULT createShaderFromMemory(ID3D11Device* device,
			ID3D11DeviceContext* context,
			const char* slsource,
			D3D11_INPUT_ELEMENT_DESC layoutdesc[],
			unsigned int numElements);

		HRESULT createShaderFromFile(ID3D11Device* device,
			ID3D11DeviceContext* context,
			const wchar_t* filename,
			D3D11_INPUT_ELEMENT_DESC layoutdesc[],
			unsigned int numElements);
		HRESULT createShaderFromMemory(ID3D11Device* device,
			ID3D11DeviceContext* context,
			const wchar_t* slsource,
			D3D11_INPUT_ELEMENT_DESC layoutdesc[],
			unsigned int numElements);

		HRESULT bindShader(ID3D11Device* device,
			ID3D11DeviceContext* context);
	private:
		ID3D11VertexShader* shader;
		ID3D11InputLayout* layout;
	};
}
