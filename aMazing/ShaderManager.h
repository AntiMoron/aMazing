#pragma once

#include<vector>
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
		unsigned int layoutCount);

	ShaderPair& getPair(const std::size_t index);
private:
	ShaderManager();
	~ShaderManager();

	static ShaderManager* instance;
	std::vector<ShaderPair> vec;
};

#define SHADERS (ShaderManager::getInstance())