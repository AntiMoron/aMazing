#pragma once

#include<Windows.h>
#define MAX_INCLUDES 9U

class ShaderInclude
{
private:
	struct WrapInclude
	{
		HANDLE hFile;
		HANDLE hFileMap;
		LARGE_INTEGER fileSize;
		void* pMappedData;
	};
	size_t includeCount;
	WrapInclude includeFiles[MAX_INCLUDES];
public:
	ShaderInclude()
	{
		for (unsigned int i = 0;i < MAX_INCLUDES;i++)
		{
			includeFiles[i].hFile = INVALID_HANDLE_VALUE;
			includeFiles[i].hFileMap = INVALID_HANDLE_VALUE;
			includeFiles[i].pMappedData = nullptr;
		}
		includeCount = 0;
	}
	~ShaderInclude()
	{
		for (auto i = 0;i < includeCount;i++)
		{
			UnmapViewOfFile(includeFiles[i].pMappedData);
			if (includeFiles[i].hFileMap != INVALID_HANDLE_VALUE)
			{
				CloseHandle(includeFiles[i].hFileMap);
			}
			if (includeFiles[i].hFile != INVALID_HANDLE_VALUE)
			{
				CloseHandle(includeFiles[i].hFile);
			}
		}
		includeCount = 0;
	}
	
	bool open(D3D_INCLUDE_TYPE)
	{
		;
	}
};