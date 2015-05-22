#pragma once
#include"aJsonHelper.hpp"
#include"aJsonData.hpp"
#include"../../containers/VirtualString.hpp"
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
			static void splitJsonString(const VirtualString& src, VirtualString** key, VirtualString** value) throw (ParamException)
			{
				size_t cur = 0;
				size_t keyStart = 0;
				size_t keyEnd = 0;
				while (cur < src.size() && aMazing::isBlank(src[cur]))
				{
					++cur;
				}
				if (src[keyStart] != '\"')
				{
					throw ParamException();//Format error.
				}
				++cur;
				keyStart = cur;
				while (cur < src.size() && src[cur] != '\"')
				{
					++cur;
				}
				keyEnd = cur - 1;
				if (keyStart >= keyEnd)
				{
					throw ParamException();//Key Error
				}
				*key = new VirtualString(src, keyStart, keyEnd);
				VirtualString tValue(src, cur, src.length());
				tValue.trim();
				if (tValue[0] == ':')
				{
					throw ParamException();//Format error.
				}
				*value = new VirtualString(tValue.subString(1).trim());
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

			static std::shared_ptr<aJsonNode> parseRecursively(const VirtualString& str) throw(ParamException)
			{
				std::shared_ptr<aJsonNode> result = std::make_shared<aJsonNode>();
				str.trim();
				if (!isSymbolsMatched(str[0], str.back()))
				{
					throw ParamException();
				}
				VirtualString strippedString = VirtualString(str, 1, str.length() - 2);
				if (strippedString.empty())
				{
					throw ParamException();
				}
				std::vector<VirtualString> splitedStrings = getContentDivided(strippedString);
				for (const VirtualString& singleString : splitedStrings)
				{
					VirtualString* key = nullptr;
					VirtualString* value = nullptr;
					splitJsonString(singleString, &key, &value);
					if (!key || !value)
					{
						throw ParamException();
					}
					result->setJsonType(parseJsonType(*value));
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
