#pragma once
#include"ShaderCompiler.hpp"
#include "ShaderObject.hpp"
namespace aMazing
{
	class VertexShaderObject : public ShaderObject
	{
	public:
		VertexShaderObject();
		~VertexShaderObject();

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
