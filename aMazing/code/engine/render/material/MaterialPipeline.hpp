#pragma once
#include"../../../common/CommonDxSupport.hpp"
#include"../../../common/CommonTemplates.hpp"
#include"../../system/MaterialType.hpp"
#include"../../shader/ShaderManager.hpp"
#include<type_traits>
#include<unordered_map>


namespace aMazing
{
	class MaterialPipeline
	{
	private:
		MaterialPipeline(){}
		~MaterialPipeline(){}
	public:
		static MaterialPipeline& getInstance()
		{
			static MaterialPipeline instance;
			return instance;
		}
		//bind user-defined enumeration to specified shaders.
		template<typename T>
		const bool addUserDefinedMaterialBind(std::enable_if<std::is_enum<T>::value, >::type materialType,
			ShaderPair& shaderPair) aNOEXCEPT
		{
			if (useredDefinedMaterial2ShaderPair.find() != useredDefinedMaterial2ShaderPair.end())
			{
				return false;
			}
			useredDefinedMaterial2ShaderPair.insert(std::pair<std::size_t, ShaderPair*>(materialType, aAddressOf(shaderPair)));
			return true;
		}

		const bool addMaterialBind(RenderTypeEnum::MATERIAL_TYPE type, ShaderPair& shaderPair) aNOEXCEPT
		{
			if (matchMaterialType2ShaderPair.find(type) != matchMaterialType2ShaderPair.end())
			{
				return false;
			}
			matchMaterialType2ShaderPair.insert(std::make_pair(type, aAddressof(shaderPair)));
			return true;
		}
		//get shader pair with given user defined material type
		template<typename T>
		const ShaderPair* getUserDefinedMaterialBind(std::enable_if<std::is_enum<T>::value>::type materialType)
		{
			auto& bindMap = useredDefinedMaterial2ShaderPair;
			auto it = bindMap.find(materialType);
			if (it != bindMap.end())
			{
				return it->second;
			}
			return nullptr;
		}

		const ShaderPair* getDefaultMaterialBind(RenderTypeEnum::MATERIAL_TYPE materialType)
		{
			auto& bindMap = matchMaterialType2ShaderPair;
			auto it = bindMap.find(materialType);
			if (it != bindMap.end())
			{
				return it->second;
			}
			return nullptr;
		}
	private:
		std::unordered_map < std::size_t, ShaderPair*> useredDefinedMaterial2ShaderPair;
		std::unordered_map < RenderTypeEnum::MATERIAL_TYPE, ShaderPair*> matchMaterialType2ShaderPair;
	};
}
