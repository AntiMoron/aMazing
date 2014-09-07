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
		param->Shutdown();
		delete param;
		param = nullptr;
	}
	vec.clear();
}

HRESULT ShaderManager::addPair(ID3D11Device* device,
	ID3D11DeviceContext* context, 
	MutableString&& vfilename,
	MutableString&& pfilename,
	D3D11_INPUT_ELEMENT_DESC layout[],
	unsigned int layoutCount,
	std::string&& shadername)
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
	ShaderPair* newpair = new ShaderPair(&v,&p,std::move(shadername));
	vec.insert(std::upper_bound(vec.begin(), vec.end(), newpair,
		[&](const ShaderPair* a, const ShaderPair* b)->bool{return (*a) < (*b); })
		, newpair);
	return S_OK;
}

ShaderPair& ShaderManager::getPair(const std::string& str)
{
	auto f = [&](const std::string& str)->long
	{
		long l = 0 ,r = vec.size() - 1,mid;
		while (l <= r)
		{
			mid = (l + r) / 2;
			if (*vec[l] == str)
			{
				return l;
			}
			if (*vec[r] == str)
			{
				return r;
			}
			if (*vec[mid] == str)
			{
				return mid;
			}
			if (*vec[mid] < str)
			{
				l = mid + 1;
			}
			else
			{
				r = mid - 1;
			}
		}
		return -1;
	};
	long index = f(str);
	if (index >= vec.size() 
		|| (index < 0))
	{
		throw std::out_of_range("Shader Manager access out of range.");
	}
	return *vec[index];
}