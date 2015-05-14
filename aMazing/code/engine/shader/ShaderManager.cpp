#include "ShaderManager.hpp"
using namespace aMazing;

ShaderManager& ShaderManager::getInstance()
{
	static ShaderManager instance;
	return instance;
}

ShaderManager::ShaderManager()
{
	bind_enabled = true;
}


ShaderManager::~ShaderManager(){}

HRESULT ShaderManager::addPair(ID3D11Device* device,
	ID3D11DeviceContext* context, 
	const char* vfilename,
	const char* pfilename,
	D3D11_INPUT_ELEMENT_DESC layout[],
	unsigned int layoutCount,
	std::string&& shadername)
{
	HRESULT hr;
	VertexShaderObject* v = new VertexShaderObject;
	hr = v->createShaderFromFile(device,context,vfilename,layout,layoutCount);
	if (FAILED(hr))
	{
		aDBG("Error At : " << vfilename);
		return hr;
	}
	PixelShaderObject* p = new PixelShaderObject;
	hr = p->createShaderFromFile(device,context,pfilename);
	if (FAILED(hr))
	{
		aDBG("Error At : " << pfilename);
		return hr;
	}
	std::shared_ptr<ShaderPair> newpair(new ShaderPair(&v,&p,std::move(shadername)));
	vec.insert(std::upper_bound(vec.begin(), vec.end(), newpair,
		[&](const std::shared_ptr<ShaderPair>& a,const std::shared_ptr<ShaderPair>& b)->bool{return (*a) < (*b); })
		, newpair);
	return S_OK;
}

bool ShaderManager::bindPair(const std::string& str, 
	ID3D11Device* device, 
	ID3D11DeviceContext* context)
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
	if (size_t(index) >= vec.size() 
		|| (index < 0))
	{
		throw std::out_of_range("Shader Manager access out of range.");
	}
	if (bind_enabled == true)
	{
		vec[index]->bindShader(device, context);
	}
	else
	{
		return false;
	}
	return true;
}

void ShaderManager::DisableShaderBind()
{
	if (bind_enabled == false)
	{
		aDBG("ShaderBinding Already Disabled.\r\n");
	}
	bind_enabled = false;
}
void ShaderManager::EnableShaderBind()
{
	if (bind_enabled == true)
	{
		aDBG("ShaderBinding Already Enabled.\r\n");
	}
	bind_enabled = true;
}
