#pragma once
#include <memory>
#include "../../common/CommonUtil.hpp"
#include "../shader/ShaderInclude.hpp"
#include "../system/file/FileSystem.hpp"
#include"../containers/Directory.hpp"
#include "../system/MutableString.hpp"
#define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)

namespace aMazing
{
	class ShaderCompiler
	{
	private:
		typedef std::unordered_map<aString,int,aString::hash> FileLevelMap;
		//returns the max depth of included file tree.
		static int shaderInitialize(
			int recuresiveLevel,
			FileLevelMap& fileLevelMap,
			const aString& fileName);
	public:
		static HRESULT compileFromFile(const char* fileName,
			const char* entryPoint,
			const char* shaderTarget,
			ID3DBlob** output);

		static HRESULT compileString(const char* str,
			const char* entryPoint,
			const char* shaderTarget,
			ID3DBlob** output);
	};
}
