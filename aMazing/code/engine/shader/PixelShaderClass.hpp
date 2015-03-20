#pragma once
#include "ShaderClass.hpp"
namespace aMazing
{
	class PixelShaderClass : public ShaderClass
	{
	public:
		PixelShaderClass();
		~PixelShaderClass();

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
