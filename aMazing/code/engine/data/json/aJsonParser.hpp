#pragma once
#include"aJsonHelper.hpp"
#include"aJsonData.hpp"

namespace aMazing
{
	namespace json
	{
		class JsonParser
		{
		private:
			static std::shared_ptr<aJsonNode> parseRecursively()
			{
				;
			}
		public:
			static std::shared_ptr<aJsonNode> getJsonRoot(const std::string& str)
			{
				std::shared_ptr<aJsonNode> result = std::make_shared<aJsonNode>();
				;
				return result;
			}
		};
	}
}
