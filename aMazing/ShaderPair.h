#pragma once
#include"VertexShaderClass.h"
#include"PixelShaderClass.h"
#include<memory>

class ShaderPair
{
public:
	~ShaderPair();
	
	ShaderPair(VertexShaderClass** pv,
		PixelShaderClass** pp,std::string && give);

	HRESULT Shutdown();

	HRESULT bindShader(ID3D11Device* device,
		ID3D11DeviceContext* context);

	bool operator < (const ShaderPair& other)const;
	bool operator < (const std::string& other)const;
	bool operator == (const std::string& other)const;

private:
	std::string shaderName;
	VertexShaderClass* pVert;
	PixelShaderClass*  pPixl;
};