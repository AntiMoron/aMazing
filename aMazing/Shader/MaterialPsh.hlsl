interface iMateral
{
	//get the ambient color of material
	float3 getAmbientColor(float2 vTexcoord);
	//get the diffuse color of material
	float3 getDiffuseColor(flaot2 vTexcoord);
	//get the specular factor
	int getSpecularFactor();
};

class cBaseMaterial : iMaterial
{
	float3 vColor;
	int iSpecFactor;
	float3 getAmbientColor(flaot2 vTexcoord)
	{
		return vColor;
	}
	float3 getDiffuseColor(float2 vTexcoord)
	{
		return (float3)vColor;
	}
	int getSpecularFactor()
	{
		return iSpecFactor;
	}
};

class cSmoothMaterial : cBaseMaterial
{
	;
};

class cSmoothTexturedMaterial : cSmoothMaterial
{
	float3 getAmbientColor(float2 vTexcoord);
	float3 getDiffuseColor(float2 vTexcoord);
};

class cPlasticLightingOnlyMaterial : cBaseMaterial
{
	float3 getAmbientColor(float2 vTexcoord)
	{
		return (float3)1.0f;
	}
	float3 getDiffuseColor(float2 vTexcoord)
	{
		return (float3)1.0f;
	}
};

class cRoughMaterial :cBaseMaterial
{
	int getSpecularFactor()
	{
		return iSpecFactor;
	}
};



HRESULT CompileShaderFromFile(WCHAR* szFileName, DWORD flags, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	// find the file
	WCHAR str[MAX_PATH];
	WCHAR workingPath[MAX_PATH], filePath[MAX_PATH];
	WCHAR *strLastSlash = NULL;
	bool  resetCurrentDir = false;

	// Get the current working directory so we can restore it later
	UINT nBytes = GetCurrentDirectory(MAX_PATH, workingPath);
	if (nBytes == MAX_PATH)
	{
		return E_FAIL;
	}

	// Check we can find the file first
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, szFileName));

	// Check if the file is in the current working directory
	wcscpy_s(filePath, MAX_PATH, str);

	strLastSlash = wcsrchr(filePath, TEXT('\\'));
	if (strLastSlash)
	{
		// Chop the exe name from the exe path
		*strLastSlash = 0;

		SetCurrentDirectory(filePath);
		resetCurrentDir = true;
	}

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

	// Create an Include handler instance
	CIncludeHandler* pIncludeHandler = new CIncludeHandler;

	// Compile the shader using optional defines and an include handler for header processing
	ID3DBlob* pErrorBlob;
	hr = D3DCompile(pFileData, FileSize.LowPart, "none", NULL, static_cast< ID3DInclude* > (pIncludeHandler),
		szEntryPoint, szShaderModel, flags, D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS, ppBlobOut, &pErrorBlob);

	delete pIncludeHandler;
	delete[]pFileData;

	// Restore the current working directory if we need to 
	if (resetCurrentDir)
	{
		SetCurrentDirectory(workingPath);
	}


	if (FAILED(hr))
	{
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		SAFE_RELEASE(pErrorBlob);
		return hr;
	}
	SAFE_RELEASE(pErrorBlob);

	return hr;
}
