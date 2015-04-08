#pragma once
#include"aJsonHelper.hpp"
#include"aJsonData.hpp"
#include"../VirtualString.hpp"
#include"../../../common/CommonFunction.hpp"

namespace aMazing
{
	namespace json
	{
		class JsonParser
		{
		private:
			static std::vector<VirtualString> getContentDivided(const VirtualString& str) throw(ParamException)
			{
				std::vector<VirtualString> result;
				result = str.splitString("");
				return result;
			}
			/*
			{},[],(),""
			*/
			static bool isSymbolsMatched(char a, char b)
			{
				if (a == '{')
				{
					return b == '}';
				}
				else if (a == '[')
				{
					return b == ']';
				}
				else if (a == '(') 
				{
					return b == ')';
				}
				else if (a == '\"')
				{
					return b == a;
				}
				//true default.
				return true;
			}
			static void splitJsonString(const VirtualString& src, VirtualString& key, VirtualString& value) throw (ParamException)
			{
				size_t startPos = 0;
				size_t endPos = src.size() - 1;
				size_t keyStart = 0;
				size_t keyEnd = 0;
				while (startPos < src.size() && aMazing::isBlank(src[startPos]))
				{
					++startPos;
				}
				if (src[keyStart] != '\"')
				{
					throw ParamException();
				}
				++startPos;
				keyStart = startPos;
				while (startPos < src.size() && src[startPos] != '\"')
				{
					startPos = ;
				}
			}

			//The string must be trimed. And be tightly of only one json data's content(without key name).
			static aJsonType parseJsonType(const VirtualString& str) throw(ParamException)
			{
				aJsonType result = aJsonType::aJsonUnknown;
				//check the key part.
				size_t startPos = 0;
				size_t endPos = str.size() - 1;
				while (startPos < str.size() && aMazing::isBlank(str[startPos]))
				{
					++startPos;
				}
				//check the value part.
				while (endPos < str.size() && aMazing::isBlank(str[endPos]))
				{
					--endPos;
				}
				++endPos;
				if (startPos >= endPos || !isSymbolsMatched(str[startPos], str[endPos]))
				{
					throw ParamException();
				}
				switch (str[startPos])
				{
				case '{':
					result = aJsonType::aJsonObject;
					break;
				case '[':
					result = aJsonType::aJsonArray;
					break;
				case '\"':
					result = aJsonType::aJsonString;
					break;
				default:
					if (str[startPos] == 't' || 
						str[startPos] == 'f' ||
						str[startPos == 'T'] ||
						str[startPos == 'F'])
					{
						result = aJsonType::aJsonBoolean;
						break;
					}
					else
					{
						try
						{
							double x = std::stod(str.subString(startPos, endPos).toStlString());
							result = aJsonType::aJsonNumber;
							break;
						}
						catch (...)
						{
							throw ParamException();
						}
					}
					throw ParamException();
				}
				return result;
			}

			static std::shared_ptr<aJsonNode> parseRecursively(const VirtualString& str)
			{
				std::shared_ptr<aJsonNode> result = std::make_shared<aJsonNode>();
				result->setJsonType(parseJsonType());
				for ()
				{
					std::shared_ptr<aJsonNode> child = std::make_shared<aJsonNode>();
					result->addChild(child);
				}
				return result;
			}
		public:
			static std::shared_ptr<aJsonNode> getJsonRoot(const std::string& str)
			{
				std::shared_ptr<aJsonNode> result = std::make_shared<aJsonNode>();
				VirtualString vs(str, 0, str.length());
				result->addChild(parseRecursively(vs));
				return result;
			}
		};
	}
}
