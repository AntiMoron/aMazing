#pragma once
#include<stack>
#include<unordered_map>
#include"ShaderPair.hpp"
#include"../../common/CommonDef.hpp"
#include"../containers/MutableString.hpp"
#include"../system/thread/aThreadSafeSingleton.hpp"
#include"../data/vertex.hpp"

namespace aMazing
{
	class ShaderManager : public aThreadSafeSingleton<ShaderManager>
	{
	private:
		friend class aThreadSafeSingleton<ShaderManager>;
		ShaderManager(){}
		std::unordered_map<std::string, std::shared_ptr<ShaderPair> > shaderStorage;
		std::stack<std::string> runtimeShaderStack;
		size_t basicShaderCount;
	public:
		template<typename VertexType>
		typename std::enable_if<std::is_base_of<aMazing::detail::VirtualVertexBase, VertexType>::value, HRESULT>::type
			addPairFromMemory(ID3D11Device* device,
			const char* vContent,
			const char* pContent,
			std::string&& shaderName,
			bool isDefault)
		{
			HRESULT hr = E_FAIL;
			auto it = shaderStorage.find(shaderName);
			if (it != shaderStorage.end())
			{
				return E_FAIL;
			}
			std::shared_ptr<ShaderPair> newpair = 
				aMakeShaderPairFromMemory(device, vContent, pContent, VertexType::input_layout);
			if (!newpair->isValid())
			{
				return E_FAIL;
			}
			shaderStorage[shaderName] = newpair;
			if (isDefault)
			{
				if (runtimeShaderStack.empty())
					runtimeShaderStack.push(shaderName);
				else
					return E_FAIL;
			}
			return S_OK;
		}

		template<typename VertexType>
		typename std::enable_if<std::is_base_of<aMazing::detail::VirtualVertexBase, VertexType>::value, HRESULT>::type
			addPairFromFile(ID3D11Device* device,
			const char* vFileName,
			const char* pFileName,
			std::string&& shaderName,
			bool isDefault)
		{
			HRESULT hr = E_FAIL;
			auto it = shaderStorage.find(shaderName);
			if (it != shaderStorage.end())
			{
				return E_FAIL;
			}
			std::shared_ptr<ShaderPair> newpair =
				aMakeShaderPairFromFile(device, vFileName, pFileName, VertexType::input_layout);
			if (!newpair->isValid())
			{
				return E_FAIL;
			}
			shaderStorage[shaderName] = newpair;
			if (isDefault)
			{
				if (runtimeShaderStack.empty())
					runtimeShaderStack.push(shaderName);
				else
					return E_FAIL;
			}
			return S_OK;
		}

		const ShaderPair& getShaderPairByTag(const std::string& tag) 
			throw(std::out_of_range)
		{
			auto it = shaderStorage.find(tag);
			if (it == shaderStorage.end())
			{
				throw std::out_of_range("This Shader Tag is Not included.");
			}
			return *(it->second);
		}

		void push(const std::string& name, ID3D11DeviceContext* context)
		{
			runtimeShaderStack.push(name);
			getShaderPairByTag(name).bindShader(context);
		}

		bool pop(ID3D11DeviceContext* context)
		{
			if (runtimeShaderStack.size() <= 1)
			{
				assert("invalid pop.Push & Pop not match");
			}
			runtimeShaderStack.pop();
			getShaderPairByTag(runtimeShaderStack.top()).bindShader(context);
			return true;
		}
	};
}

#define SHADERS (aMazing::ShaderManager::getInstance())
