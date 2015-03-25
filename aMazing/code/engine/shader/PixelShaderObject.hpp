#pragma once
#include "ShaderObject.hpp"
namespace aMazing
{
	class PixelShaderObject : public ShaderObject
	{
	public:
		PixelShaderObject();
		~PixelShaderObject();

		HRESULT createShaderFromFile(ID3D11Device* device,
			ID3D11DeviceContext* context,
			const char* filename);
		HRESULT createShaderFromMemory(ID3D11Device* device,
			ID3D11DeviceContext* context,
			const char* slsource);

		HRESULT createShaderFromFile(ID3D11Device* device,
			ID3D11DeviceContext* context,
			const wchar_t* filename);
		HRESULT createShaderFromMemory(ID3D11Device* device,
			ID3D11DeviceContext* context,
			const wchar_t* slsource);

		HRESULT bindShader(ID3D11Device* device,
			ID3D11DeviceContext* context);
	private:
		ID3D11PixelShader* shader;
	};
}
