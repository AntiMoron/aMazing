#pragma once
#include "ShaderClass.h"
class PixelShaderClass :
	public ShaderClass
{
public:
	PixelShaderClass();
	~PixelShaderClass();

	void Shutdown();

	HRESULT createShaderFromFile(ID3D11Device* device,
		ID3D11DeviceContext* context,
		MutableString & filename);
	HRESULT createShaderFromMemory(ID3D11Device* device,
		ID3D11DeviceContext* context,
		MutableString & slsource);

	HRESULT createShaderFromFile(ID3D11Device* device,
		ID3D11DeviceContext* context,
		MutableString && filename);
	HRESULT createShaderFromMemory(ID3D11Device* device,
		ID3D11DeviceContext* context,
		MutableString && slsource);

	HRESULT bindShader(ID3D11Device* device,
		ID3D11DeviceContext* context);
private:
	ID3D11PixelShader* shader;
};

