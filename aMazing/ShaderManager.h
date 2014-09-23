#pragma once

#include<vector>
#include<algorithm>
#include"ShaderPair.h"
#include"MutableString.h"

class ShaderManager
{
public:
	static ShaderManager& getInstance();

	HRESULT addPair(ID3D11Device* device,
		ID3D11DeviceContext* context,
		MutableString&& vfilename,
		MutableString&& pfilename,
		D3D11_INPUT_ELEMENT_DESC layout[],
		unsigned int layoutCount,
		std::string&& shadername);

	bool bindPair(const std::string& str,
		ID3D11Device* device,
		ID3D11DeviceContext* context);
	
	void DisableShaderBind();
	void EnableShaderBind();
private:
	ShaderManager();
	~ShaderManager();

	static ShaderManager* instance;
	std::vector<ShaderPair*> vec;
	bool bind_enabled;
};

#define SHADERS (ShaderManager::getInstance())