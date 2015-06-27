#pragma once
#include"../engine/exception/ConfigException.hpp"
#include"../engine/shader/ShaderManager.hpp"
#include"../engine/system/D3DManager.hpp"
#include"../engine/data/vertex.hpp"
#include"data/xml/aXmlParser.hpp"
namespace aMazing
{
	class LoadConfig : public aThreadSafeSingleton<LoadConfig>
	{
	private:
		friend class aThreadSafeSingleton<LoadConfig>;
		explicit LoadConfig() aNOEXCEPT{}
	public:
		void loadConfig() throw(FailureException, ConfigException)
		{
			aXmlParser parser;
			std::shared_ptr<aXmlNode> configXml = parser.parseFile("config.xml");
			//Judge whether there's only 1 'shaders' div
			std::vector<aXmlNode*> shadersVec = configXml->getChildren("shaders");
			if (shadersVec.size() != 1)
			{
				throw ConfigException("Config file does not have exact one \'shaders\' div.");
			}
			aXmlNode* shaders = shadersVec[0];
			std::vector<aXmlNode*> shaderVec = shaders->getChildren("shader");
			if (shaderVec.size() != shaders->children.size())
			{
				throw ConfigException("Error:Only \'shader\' div can exist in \'shaders\' div.");
			}
			bool defaultShaderFound = false;
			for (aXmlNode* p : shaderVec)
			{
				aXmlNode& lsh = *p;
				std::string vertexPath;
				std::string pixelPath;
				std::string layoutName;
				bool isDefault = false;				
				try
				{
					if (lsh["isDefault"].value == "true")
					{
						if (defaultShaderFound)
						{
							throw ConfigException("Error: Only one default shader can be set.");
						}
						defaultShaderFound = true;
						isDefault = true;
					}
					else
					{
						if (lsh["isDefault"].value != "false")
							throw ConfigException("Error: Only \'true\' and "
							"\'false\' can be accepted in attribute \'isDefault\'.");
					}
				}
				catch (const FailureException&)
				{
					;
				}
				vertexPath = lsh["vsrc"].value;
				pixelPath = lsh["psrc"].value;
				layoutName = lsh["layoutName"].value;
				VertexType vertexType = detail::getVertexTypeByLayoutName(layoutName);
				addFileFromFile(vertexType,
					vertexPath.c_str(),
					pixelPath.c_str(),
					std::forward<std::string>(lsh.content));
			}
		}
		HRESULT loadConfigAndPop()
		{
			try
			{
				loadConfig();
			}
			catch (const FailureException& e)
			{
				MessageBoxA(nullptr, e.what(), "Config Error", MB_OK);
				return E_FAIL;
			}
			catch (const ConfigException& e)
			{
				MessageBoxA(nullptr, e.what(), "Config Error", MB_OK);
				return E_FAIL;
			}
			catch (...)
			{
				MessageBoxA(nullptr, "Unknown Exception", "Config Error", MB_OK);
				return E_FAIL;
			}
			return S_OK;
		}
	private:
		void addFileFromFile(VertexType vertexType, const char* vFile, const char* pFile,std::string&& shaderName) throw(FailureException)
		{
			switch (vertexType)
			{
				case VERTEX_TYPE:
					SHADERS.addPairFromFile<Vertex>(D3DManager::getDevice(MANAGED_DEVICE_TYPE::DEFAULT_DEVICE),
						vFile,
						pFile,
						std::move(shaderName));
					return;
				case SKIN_VERTEX_TYPE:
					SHADERS.addPairFromFile<SkinVertex>(D3DManager::getDevice(MANAGED_DEVICE_TYPE::DEFAULT_DEVICE),
						vFile,
						pFile,
						std::move(shaderName));
					return;
			}
			throw FailureException("Config Error : Invalid Shader Param.");
		}
	};
}
#define CONFIG (aMazing::LoadConfig::getInstance())