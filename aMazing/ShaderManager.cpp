#include "ShaderManager.h"


ShaderManager* ShaderManager::instance = nullptr;

ShaderManager& ShaderManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new ShaderManager;
	}
	return *instance;
}

ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
	for (auto& param : vec)
	{
		param.Shutdown();
	}
	vec.clear();
}




HRESULT ShaderManager::addPair(ID3D11Device* device,
	ID3D11DeviceContext* context, 
	MutableString&& vfilename,
	MutableString&& pfilename,
	D3D11_INPUT_ELEMENT_DESC layout[],
	unsigned int layoutCount)
{
	HRESULT hr;
	VertexShaderClass* v = new VertexShaderClass;
	hr = v->createShaderFromFile(device,context,vfilename,layout,layoutCount);
	if (FAILED(hr))
	{
		return hr;
	}
	PixelShaderClass* p = new PixelShaderClass;
	hr = p->createShaderFromFile(device,context,pfilename);
	if (FAILED(hr))
	{
		return hr;
	}
	ShaderPair* newpair = new ShaderPair(&v,&p);
	vec.push_back(*newpair);
}

ShaderPair& ShaderManager::getPair(const std::size_t index)
{
	return vec[index];
}