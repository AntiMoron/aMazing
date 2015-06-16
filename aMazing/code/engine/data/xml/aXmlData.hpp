#pragma once
#include <memory>
#include <vector>
#include <string>
#include "aXmlConfig.hpp"
#include "../../../common/CommonDef.hpp"
#include "../../../common/CommonFunction.hpp"

namespace aMazing
{
	enum aXmlEncoding
	{
		US_ASCII,
		UTF_8,
		UTF_16,
		ISO_8859_1
	};

	struct aXmlAttribute
	{
		std::string key;
		std::string value;
	};

	struct aXmlNode
	{
		aXmlNode()
		{
			parent = nullptr;
		}
#ifdef EXPAT_IMPL
		aXmlNode(const char* name, 
			const char** atts)
		{
			parent = nullptr;
			key = name;
		}
#endif
		~aXmlNode()
		{
			for (aXmlNode* child : children)
			{
				aSAFE_DELETE(child);
			}
		}
		aXmlNode* parent;
		std::vector<aXmlNode* > children;
		std::vector<aXmlAttribute > attributes;
		std::string key;
		std::string content;
	};
}
