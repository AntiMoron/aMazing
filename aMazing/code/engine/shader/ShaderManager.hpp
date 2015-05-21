#pragma once

#include<vector>
#include"ShaderPair.hpp"
#include"../../common/CommonDef.hpp"
#include"../containers/MutableString.hpp"
#include"../system/thread/aThreadSafeSingleton.hpp"
namespace aMazing
{
	class ShaderManager : public aThreadSafeSingleton<ShaderManager>
	{
	private:
		friend class aThreadSafeSingleton<ShaderManager>;
		ShaderManager(){}

		std::vector<std::shared_ptr<ShaderPair> > vec;
		size_t basicShaderCount;
	public:
		template<typename VertexType>
		typename std::enable_if<std::is_base_of<detail::VirtualVertexBase, VertexType>::value, HRESULT>::type
			addPairFromMemory(ID3D11Device* device,
			const char* vContent,
			const char* pContent,
			std::string&& shaderName)
		{
			HRESULT hr = E_FAIL;
			std::shared_ptr<ShaderPair> newpair = 
				aMakeShaderPairFromMemory(device, vContent, pContent, VertexType::input_layout, shaderName);
			if (!newpair->isValid())
			{
				return E_FAIL;
			}
			vec.insert(std::upper_bound(vec.begin(), vec.end(), newpair,
				[](const std::shared_ptr<ShaderPair>& a, const std::shared_ptr<ShaderPair>& b)->bool{return (*a) < (*b); })
				, newpair);
			return S_OK;
		}

		template<typename VertexType>
		typename std::enable_if<std::is_base_of<detail::VirtualVertexBase, VertexType>::value, HRESULT>::type
			addPairFromFile(ID3D11Device* device,
			const char* vFileName,
			const char* pFileName,
			std::string&& shaderName)
		{
			HRESULT hr = E_FAIL;
			std::shared_ptr<ShaderPair> newpair =
				aMakeShaderPairFromFile(device, vFileName, pFileName, VertexType::input_layout, shaderName);
			if (!newpair->isValid())
			{
				return E_FAIL;
			}
			vec.insert(std::upper_bound(vec.begin(), vec.end(), newpair,
				[](const std::shared_ptr<ShaderPair>& a, const std::shared_ptr<ShaderPair>& b)->bool{return (*a) < (*b); })
				, newpair);
			return S_OK;
		}

		const ShaderPair& getShaderPairByTag(const std::string& tag) 
			throw(std::out_of_range)
		{
			auto f = [this](const std::string& str)->long
			{
				long l = 0, r = vec.size() - 1, mid;
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
			long index = f(tag);
			if (size_t(index) >= vec.size()
				|| (index < 0))
			{
				throw std::out_of_range("Shader Manager access out of range.");
			}
			return *vec[index];
		}

		void bindPair(const std::string& tag,
			ID3D11DeviceContext* context)
		{
			getShaderPairByTag(tag).bindShader(context);
		}

		void push(const std::string& name)
		{
			vec.push_back(
				std::make_shared<ShaderPair>(getShaderPairByTag(name)));
		}

		void push(const ShaderPair& p)
		{
			vec.push_back(std::make_shared<ShaderPair>(p));
		}


		bool pop()
		{
			if (vec.size() < basicShaderCount)
			{
				aDBG("invalid pop.Push & Pop not match");
				throw;
			}
			vec.pop_back();
			return true;
		}
	};
}

#define SHADERS (aMazing::ShaderManager::getInstance())
