#include "ShaderCompilerClass.hpp"
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


ShaderCompilerClass::ShaderCompilerClass()
{
}

ShaderCompilerClass::~ShaderCompilerClass()
{
	;
}

HRESULT ShaderCompilerClass::compileFromFile(const char* filename,
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

	// open the file
	HANDLE hFile = CreateFile(str, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	// Get the file size
	LARGE_INTEGER FileSize;
	GetFileSizeEx(hFile, &FileSize);

	// create enough space for the file data
	BYTE* pFileData = new BYTE[FileSize.LowPart];
	if (!pFileData)
		return E_OUTOFMEMORY;

	// read the data in
	DWORD BytesRead;
	if (!ReadFile(hFile, pFileData, FileSize.LowPart, &BytesRead, NULL))
		return E_FAIL;

	CloseHandle(hFile);

	ShaderInclude* pIncludeHandler = new ShaderInclude;
	ID3DBlob* errorMsg = nullptr;

	auto lastSplashInd = wFilePath.rfind(TEXT('\\'));
	auto lastSplashInd2 = wFilePath.rfind(TEXT('/'));

	if (lastSplashInd != std::wstring::npos || lastSplashInd2 != std::wstring::npos)
	{
		if (lastSplashInd == std::wstring::npos)
		{
			lastSplashInd = lastSplashInd2;
		}
		else if (lastSplashInd2 != std::wstring::npos)
		{
			lastSplashInd = aMin<decltype(lastSplashInd)>(lastSplashInd, lastSplashInd2);
		}
		wFilePath = wFilePath.substr(0, lastSplashInd);
		SetCurrentDirectoryW(wFilePath.c_str());
		resetCurrentDir = true;
	}

	hr = D3DCompile(pFileData, FileSize.LowPart, NULL, NULL, static_cast<ID3DInclude*> (pIncludeHandler),
		entryPoint, shaderTarget, 0, D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS, output, &errorMsg);
	if (!!pIncludeHandler)
	{
		delete pIncludeHandler;
	}
	if (!!pFileData)
	{
		delete[] pFileData;
	}
	if (resetCurrentDir)
	{
		SetCurrentDirectory(workingPath);
	}

	if (FAILED(hr))
	{
		MessageBoxA(NULL,reinterpret_cast<char*>(errorMsg->GetBufferPointer()),"error",MB_OK);
		if (errorMsg)
		{
			errorMsg->Release();
		}
		return hr;
	}
	if (errorMsg)
	{
		errorMsg->Release();
	}
	return S_OK;
}

HRESULT ShaderCompilerClass::compileString(const char* str
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
	std::string shaderstr = MutableString(str).getMultiByteString();
	std::cout << shaderstr << std::endl;
	hr = D3DCompile(shaderstr.c_str(),
		shaderstr.length(),
		nullptr,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint,
		shaderTarget,
		dwShaderFlags, 
		0,
		output,
		&errorMsg);
	
	if (errorMsg != nullptr)
	{
		char* errorText = new char[errorMsg->GetBufferSize()];
		memcpy(errorText, errorMsg->GetBufferPointer(), errorMsg->GetBufferSize());
		MessageBoxA(NULL,errorText,"Shader Error",MB_OK);
		errorMsg->Release();
		return E_FAIL;
	}
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
