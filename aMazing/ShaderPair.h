#pragma once
#include"VertexShaderClass.h"
#include"PixelShaderClass.h"
#include<memory>

class ShaderPair
{
public:
	ShaderPair();
	~ShaderPair();
	
	ShaderPair(VertexShaderClass** pv,
		PixelShaderClass** pp);

	HRESULT Shutdown();

	HRESULT bindShader(ID3D11Device* device,
		ID3D11DeviceContext* context);
//private:
	VertexShaderClass* pVert;
	PixelShaderClass*  pPixl;
};