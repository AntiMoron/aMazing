#include "ShaderCompiler.hpp"
using namespace aMazing;

WCHAR* aMediaSearchPath()
{
	static WCHAR s_strMediaSearchPath[MAX_PATH] = {0};
	return s_strMediaSearchPath;
}

bool aFindMediaSearchParentDirs(WCHAR* strSearchPath, int cchSearch, WCHAR* strStartAt,
	WCHAR* strLeafName)
{
	WCHAR strFullPath[MAX_PATH] = {0};
	WCHAR strFullFileName[MAX_PATH] = {0};
	WCHAR strSearch[MAX_PATH] = {0};
	WCHAR* strFilePart = NULL;

	GetFullPathName(strStartAt, MAX_PATH, strFullPath, &strFilePart);
	if (strFilePart == NULL)
		return false;

	while (strFilePart != NULL && *strFilePart != '\0')
	{
		swprintf_s(strFullFileName, MAX_PATH, L"%s\\%s", strFullPath, strLeafName);
		if (GetFileAttributes(strFullFileName) != 0xFFFFFFFF)
		{
			wcscpy_s(strSearchPath, cchSearch, strFullFileName);
			return true;
		}

		swprintf_s(strSearch, MAX_PATH, L"%s\\..", strFullPath);
		GetFullPathName(strSearch, MAX_PATH, strFullPath, &strFilePart);
	}

	return false;
}

bool aFindMediaSearchTypicalDirs(WCHAR* strSearchPath, int cchSearch, LPCWSTR strLeaf,
	WCHAR* strExePath, WCHAR* strExeName)
{
	// Typical directories:
	//      .\
	    //      ..\
	    //      ..\..\
	    //      %EXE_DIR%\
	    //      %EXE_DIR%\..\
	    //      %EXE_DIR%\..\..\
	    //      %EXE_DIR%\..\%EXE_NAME%
	//      %EXE_DIR%\..\..\%EXE_NAME%
	//      DXSDK media path

	// Search in .\  
	wcscpy_s(strSearchPath, cchSearch, strLeaf);
	if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
		return true;

	// Search in ..\  
	swprintf_s(strSearchPath, cchSearch, L"..\\%s", strLeaf);
	if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
		return true;

	// Search in ..\..\ 
	swprintf_s(strSearchPath, cchSearch, L"..\\..\\%s", strLeaf);
	if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
		return true;

	// Search in ..\..\ 
	swprintf_s(strSearchPath, cchSearch, L"..\\..\\%s", strLeaf);
	if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
		return true;

	// Search in the %EXE_DIR%\ 
	swprintf_s(strSearchPath, cchSearch, L"%s\\%s", strExePath, strLeaf);
	if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
		return true;

	// Search in the %EXE_DIR%\..\ 
	swprintf_s(strSearchPath, cchSearch, L"%s\\..\\%s", strExePath, strLeaf);
	if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
		return true;

	// Search in the %EXE_DIR%\..\..\ 
	swprintf_s(strSearchPath, cchSearch, L"%s\\..\\..\\%s", strExePath, strLeaf);
	if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
		return true;

	// Search in "%EXE_DIR%\..\%EXE_NAME%\".  This matches the DirectX SDK layout
	swprintf_s(strSearchPath, cchSearch, L"%s\\..\\%s\\%s", strExePath, strExeName, strLeaf);
	if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
		return true;

	// Search in "%EXE_DIR%\..\..\%EXE_NAME%\".  This matches the DirectX SDK layout
	swprintf_s(strSearchPath, cchSearch, L"%s\\..\\..\\%s\\%s", strExePath, strExeName, strLeaf);
	if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
		return true;

	// Search in media search dir 
	WCHAR* s_strSearchPath = aMediaSearchPath();
	if (s_strSearchPath[0] != 0)
	{
		swprintf_s(strSearchPath, cchSearch, L"%s%s", s_strSearchPath, strLeaf);
		if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
			return true;
	}

	return false;
}


//--------------------------------------------------------------------------------------
// Tries to find the location of a SDK media file
//       cchDest is the size in WCHARs of strDestPath.  Be careful not to 
//       pass in sizeof(strDest) on UNICODE builds.
//--------------------------------------------------------------------------------------
HRESULT aFindMediaFileCch(WCHAR* strDestPath, int cchDest,
	LPCWSTR strFilename)
{
	bool bFound;
	WCHAR strSearchFor[MAX_PATH];

	if (NULL == strFilename || strFilename[0] == 0 || NULL == strDestPath || cchDest < 10)
		return E_INVALIDARG;

	// Get the exe name, and exe path
	WCHAR strExePath[MAX_PATH] = {0};
	WCHAR strExeName[MAX_PATH] = {0};
	WCHAR* strLastSlash = NULL;
	GetModuleFileName(NULL, strExePath, MAX_PATH);
	strExePath[MAX_PATH - 1] = 0;
	strLastSlash = wcsrchr(strExePath, TEXT('\\'));
	if (strLastSlash)
	{
		wcscpy_s(strExeName, MAX_PATH, &strLastSlash[1]);

		// Chop the exe name from the exe path
		*strLastSlash = 0;

		// Chop the .exe from the exe name
		strLastSlash = wcsrchr(strExeName, TEXT('.'));
		if (strLastSlash)
			*strLastSlash = 0;
	}

	// Typical directories:
	//      .\
	    //      ..\
	    //      ..\..\
	    //      %EXE_DIR%\
	    //      %EXE_DIR%\..\
	    //      %EXE_DIR%\..\..\
	    //      %EXE_DIR%\..\%EXE_NAME%
	//      %EXE_DIR%\..\..\%EXE_NAME%

	// Typical directory search
	bFound = aFindMediaSearchTypicalDirs(strDestPath, cchDest, strFilename, strExePath, strExeName);
	if (bFound)
		return S_OK;

	// Typical directory search again, but also look in a subdir called "\media\" 
	swprintf_s(strSearchFor, MAX_PATH, L"media\\%s", strFilename);
	bFound = aFindMediaSearchTypicalDirs(strDestPath, cchDest, strSearchFor, strExePath, strExeName);
	if (bFound)
		return S_OK;

	WCHAR strLeafName[MAX_PATH] =
	{
		0
	};

	// Search all parent directories starting at .\ and using strFilename as the leaf name
	wcscpy_s(strLeafName, MAX_PATH, strFilename);
	bFound = aFindMediaSearchParentDirs(strDestPath, cchDest, L".", strLeafName);
	if (bFound)
		return S_OK;

	// Search all parent directories starting at the exe's dir and using strFilename as the leaf name
	bFound = aFindMediaSearchParentDirs(strDestPath, cchDest, strExePath, strLeafName);
	if (bFound)
		return S_OK;

	// Search all parent directories starting at .\ and using "media\strFilename" as the leaf name
	swprintf_s(strLeafName, MAX_PATH, L"media\\%s", strFilename);
	bFound = aFindMediaSearchParentDirs(strDestPath, cchDest, L".", strLeafName);
	if (bFound)
		return S_OK;

	// Search all parent directories starting at the exe's dir and using "media\strFilename" as the leaf name
	bFound = aFindMediaSearchParentDirs(strDestPath, cchDest, strExePath, strLeafName);
	if (bFound)
		return S_OK;

	// On failure, return the file as the path but also return an error code
	wcscpy_s(strDestPath, cchDest, strFilename);

	return E_FAIL;
}

int ShaderCompiler::shaderInitialize(
	int recuresiveLevel,
	FileLevelMap& fileLevelMap,
	const aString& fileName)
{
	int retDepth = recuresiveLevel;
	try
	{
		aString standardFileName = transformSplashes(fileName);
		aString fileDirectory = standardFileName.subString(0, fileName.rfind('/') - fileName.begin()) + '/';
		aString ret(FileSystem::readFileA(fileName.c_str()).c_str());
		aVector<aString> includePaths;
		const static aString indicatorStr = "#include";
		aString::size_type cnt = 0;
		aString::iterator pos = ret.findByIndex(cnt, indicatorStr);
		do{
			aString includePath = "";
			pos += indicatorStr.length();
			while (pos != ret.end() && isBlank(*pos))
				++pos;
			if (pos == ret.end())
				break;
			if (*pos == '\"')
				++pos;
			else
				break;
			while (pos != ret.end() && !isBlank(*pos) && ((*pos) != '\"'))
			{
				includePath += *pos;
				++pos;
			}
			includePaths.push_back(includePath);
			++cnt;
			pos = ret.findByIndex(cnt, indicatorStr);
		} while (pos != ret.end());
		if (!includePaths.empty())
		{
			for (aString& includePath : includePaths)
			{
				includePath = fileDirectory + includePath;
				includePath = aMazing::minimizeDirectory(includePath);
				if (fileLevelMap.find(includePath) == fileLevelMap.end())
				{
					fileLevelMap[includePath] = recuresiveLevel;
				}
				else
				{
					int lv = fileLevelMap[includePath];
					fileLevelMap[includePath] = aMax(lv, recuresiveLevel);
				}
				retDepth = aMax(retDepth, shaderInitialize(recuresiveLevel + 1,
					fileLevelMap,
					includePath));
			}
		}
	}
	catch (const FailureException&)
	{
		//deal with those files that can't be found.
		fileLevelMap[fileName] = INT_MAX;
	}
	return retDepth;
}

HRESULT ShaderCompiler::compileFromFile(const char* filename,
	const char* entryPoint,
	const char* shaderTarget,
	ID3DBlob** output)
{
	HRESULT hr = E_FAIL;
	bool  resetCurrentDir = false;
	// find the file
	WCHAR str[MAX_PATH];
	wchar_t workingPath[MAX_PATH], filePath[MAX_PATH];
	auto nBytes = GetCurrentDirectoryW(MAX_PATH, workingPath);
	if (nBytes >= MAX_PATH)
	{
		return E_FAIL;
	}
	auto wFileName = MutableString(filename).getWideString();
	// Check we can find the file first
	hr = aFindMediaFileCch(str, MAX_PATH, wFileName.c_str());
	if (FAILED(hr))
	{
		return hr;
	}
	// Check if the file is in the current working directory
	wcscpy_s(filePath, MAX_PATH, str);
	std::wstring wFilePath = filePath;
	SetCurrentDirectoryW(wFilePath.c_str());

	std::shared_ptr<ShaderInclude> pIncludeHandler = std::make_shared<ShaderInclude>();
	ID3DBlob* errorMsg = nullptr;

	std::string sumContent;
	FileLevelMap fileLevelMap;
	fileLevelMap.insert(std::pair<aString, int>(filename, 0));
	int maxDepth = shaderInitialize(1, fileLevelMap, filename);
	for (int i = INT_MAX; i >= 0;)
	{
		auto it = fileLevelMap.begin();
		while(it != fileLevelMap.end())
		{
			//reach the same level's header file.
			if (it->second == i)
			{
				std::string content = FileSystem::readFileA(it->first.c_str());
				const static std::string headerIndicator = "#include";
				auto firstEndPos = content.find(headerIndicator);
				auto secondStartPos = content.rfind(headerIndicator);
				//jump the header file name.
				if (secondStartPos != std::string::npos)
				{
					while (secondStartPos < content.length() &&
						(content[secondStartPos] != '\"'))
						++secondStartPos;
					++secondStartPos;
					while (secondStartPos < content.length() &&
						(content[secondStartPos] != '\"'))
						++secondStartPos;
					++secondStartPos;
				}
				std::string first, second;
				first = content.substr(0, firstEndPos);
				if (secondStartPos != std::string::npos)
					second = content.substr(secondStartPos);
				sumContent += first + second + '\n';
				it = fileLevelMap.erase(it);
			}
			else
				++it;
		}
		if (i == INT_MAX)
			i = maxDepth;
		else 
			--i;
	}

	hr = D3DCompile(sumContent.c_str(),
		sumContent.length(),
		NULL, NULL, 
		static_cast<ID3DInclude*> (pIncludeHandler.get()),
		entryPoint, shaderTarget, 
		0, D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS, 
		output, &errorMsg);
	if (resetCurrentDir)
	{
		SetCurrentDirectoryW(workingPath);
	}

	if (FAILED(hr))
	{
		MessageBoxA(NULL,reinterpret_cast<char*>(errorMsg->GetBufferPointer()),"error",MB_OK);
		aSAFE_RELEASE(errorMsg);
		return hr;
	}
	aSAFE_RELEASE(errorMsg);
	return S_OK;
}

HRESULT ShaderCompiler::compileString(const char* str
	, const char* entryPoint,
	const char* shaderTarget,
	ID3DBlob** output)
{
	HRESULT hr;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | 
		D3DCOMPILE_OPTIMIZATION_LEVEL3;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* errorMsg = nullptr;
	ID3DBlob* compiled = nullptr;
	std::string shaderStr = str;
	aDBG(shaderStr);
	hr = D3DCompile(shaderStr.c_str(),
		shaderStr.length(),
		nullptr,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint,
		shaderTarget,
		dwShaderFlags, 
		0,
		output,
		&errorMsg);
	
	if (!!errorMsg)
	{
		char* errorText = new char[errorMsg->GetBufferSize()];
		memcpy(errorText, errorMsg->GetBufferPointer(), errorMsg->GetBufferSize());
		MessageBoxA(NULL,errorText,"Shader Error",MB_OK);
		aSAFE_RELEASE(errorMsg);
		if (!!errorText)
		{
			delete[] errorText;
			errorText = nullptr;
		}
		return E_FAIL;
	}
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
