#pragma once
#include"aXmlData.hpp"
#include"../../exception/FailureException.hpp"
#ifdef EXPAT_IMPL
#include"expat.h"
#include<cstdio>

#pragma comment(lib,"libexpat.lib")
#pragma comment(lib,"libexpatMT.lib")
#pragma comment(lib,"libexpatw.lib")
#pragma comment(lib,"libexpatwMT.lib")

#ifdef XML_LARGE_SIZE
#if defined(XML_USE_MSC_EXTENSIONS) && _MSC_VER < 1400
#define XML_FMT_INT_MOD "I64"
#else
#define XML_FMT_INT_MOD "ll"
#endif
#else
#define XML_FMT_INT_MOD "l"
#endif

#endif

namespace aMazing
{
#ifdef EXPAT_IMPL
	class aXmlParser
	{
	public:
		std::shared_ptr<aXmlNode> parseFile(const std::string& fileName) 
			throw (FailureException)
		{
			XML_Parser parser = XML_ParserCreate(nullptr);
			XML_SetElementHandler(parser, startElement, endElement);
			XML_SetCharacterDataHandler(parser, dataHandler);
			aXmlNode* ret = nullptr;
			XML_SetUserData(parser, reinterpret_cast<void*>(&ret));
			std::FILE* fp = std::fopen(fileName.c_str(), "r");
			if (fp == nullptr)
			{
				throw FailureException("File not exist");
			}
			do
			{
				size_t len = fread(buf, 1, sizeof(buf), fp);
				done = len < sizeof(buf);
				if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR)
				{
					aSAFE_DELETE(ret);
					std::string exp = "XML Error at Line(";
					exp += std::to_string(XML_GetCurrentLineNumber(parser));
					exp += ")";
					exp += XML_ErrorString(XML_GetErrorCode(parser));
					throw FailureException(exp.c_str());
				}
			} while (!done);
			std::fclose(fp);
			XML_ParserReset(parser, nullptr);
			XML_ParserFree(parser);
			return std::shared_ptr<aXmlNode>(ret);
		}
		std::shared_ptr<aXmlNode> parserString(const std::string& content)
			throw (FailureException)
		{
			XML_Parser parser = XML_ParserCreate(nullptr);
			XML_SetElementHandler(parser, startElement, endElement);
			XML_SetCharacterDataHandler(parser, dataHandler);
			aXmlNode* ret = nullptr;
			XML_SetUserData(parser, reinterpret_cast<void*>(&ret));
			if (XML_Parse(parser, &content[0], content.size(), 1)
				== XML_STATUS_ERROR)
			{
				aSAFE_DELETE(ret);
				std::string exp = "XML Error at Line(";
				exp += std::to_string(XML_GetCurrentLineNumber(parser));
				exp += ")";
				exp += XML_ErrorString(XML_GetErrorCode(parser));
				throw FailureException(exp.c_str());
			}
			XML_ParserReset(parser, nullptr);
			XML_ParserFree(parser);
			return std::shared_ptr<aXmlNode>(ret);
		}
	private:
		static void XMLCALL dataHandler(void *userData, const char *content, int length)
		{
			aXmlNode** ppNode = reinterpret_cast<aXmlNode**>(userData);
			aXmlNode*& pNode = *ppNode;
			for (size_t i = 0; i < length; i++)
			{
				if (!aMazing::isBlank(content[i]))
					pNode->content += content[i];
			}
		}
		static void XMLCALL startElement(void *userData, const char *name, const char **atts)
		{
			aXmlNode** ppNode = reinterpret_cast<aXmlNode**>(userData);
			aXmlNode*& pNode = *ppNode;
			if (*ppNode == nullptr)
			{
				pNode = new aXmlNode;
			}
			else
			{
				aXmlNode* parent = pNode;
				pNode->children.push_back(new aXmlNode);
				pNode = pNode->children.back();
				pNode->parent = parent;
			}

 			pNode->key = name;
			
			int i = 0;
			while (atts[i] != nullptr)
			{
				auto& nodeAtts = pNode->attributes;
				if(nodeAtts.size() <= (i / 2))
					nodeAtts.push_back(aXmlAttribute());
				if (i % 2 == 0)
					nodeAtts[i/2].key = atts[i];
				else
					nodeAtts[i/2].value = atts[i];
				++i;
			}
		}

		static void XMLCALL endElement(void *userData, const char *name)
		{
			aXmlNode** ppNode = reinterpret_cast<aXmlNode**>(userData);
			aXmlNode*& pNode = *ppNode;
			if (pNode->parent != nullptr)
				pNode = pNode->parent;
		}

		char buf[BUFSIZ];
		int done;
	};
#endif 
}
