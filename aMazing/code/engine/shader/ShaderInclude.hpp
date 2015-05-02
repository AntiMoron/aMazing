#pragma once
#include<Windows.h>
#include<string>
#include"../../common/CommonDxSupport.hpp"

namespace aMazing
{
	class ShaderInclude : public ID3DInclude
	{
	private:
		const static unsigned int MAX_INCLUDES = 9U;
		struct sInclude
		{
			HANDLE         hFile;
			HANDLE         hFileMap;
			LARGE_INTEGER  FileSize;
			void           *pMapData;
		};

		struct sInclude   m_includeFiles[MAX_INCLUDES];
		unsigned int      m_nIncludes;

		wchar_t filePath[MAX_PATH];

		void recursivelyOpen()
		{
			;
		}

	public:
		ShaderInclude()
		{
			// array initialization
			for (unsigned int i = 0; i<MAX_INCLUDES; i++)
			{
				m_includeFiles[i].hFile = INVALID_HANDLE_VALUE;
				m_includeFiles[i].hFileMap = INVALID_HANDLE_VALUE;
				m_includeFiles[i].pMapData = NULL;
			}
			m_nIncludes = 0;
		}
		~ShaderInclude()
		{
			for (unsigned int i = 0; i<m_nIncludes; i++)
			{
				UnmapViewOfFile(m_includeFiles[i].pMapData);

				if (m_includeFiles[i].hFileMap != INVALID_HANDLE_VALUE)
					CloseHandle(m_includeFiles[i].hFileMap);

				if (m_includeFiles[i].hFile != INVALID_HANDLE_VALUE)
					CloseHandle(m_includeFiles[i].hFile);
			}
			SetCurrentDirectory(filePath);
			m_nIncludes = 0;
		}
		//get file directory from file path.
		std::string cutFilePath(const std::string& src)
		{
			int aindex = src.rfind('\\');
			int bindex = src.rfind('/');
			int index = max(aindex, bindex);
			if (index < 0)
			{
				return src;
			}
			return src.substr(0, index);
		}

		STDMETHOD(Open(
			D3D_INCLUDE_TYPE IncludeType,
			LPCSTR pFileName,
			LPCVOID pParentData,
			LPCVOID *ppData,
			UINT *pBytes
			))
		{
			unsigned int   incIndex = m_nIncludes + 1;
			aDBG(pFileName);
			auto nBytes = GetCurrentDirectoryW(MAX_PATH, filePath);
			if (nBytes >= MAX_PATH)
			{
				return E_FAIL;
			}
			
			std::string cutedFilePath = cutFilePath(pFileName);
			SetCurrentDirectoryA(cutedFilePath.c_str());
			aDBG(cutedFilePath);

			// Make sure we have enough room for this include file
			if (incIndex >= MAX_INCLUDES)
				return E_FAIL;

			// try to open the file
			m_includeFiles[incIndex].hFile = CreateFileA(pFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
				FILE_FLAG_SEQUENTIAL_SCAN, NULL);
			if (INVALID_HANDLE_VALUE == m_includeFiles[incIndex].hFile)
			{
				return E_FAIL;
			}

			// Get the file size
			GetFileSizeEx(m_includeFiles[incIndex].hFile, &m_includeFiles[incIndex].FileSize);

			// Use Memory Mapped File I/O for the header data
			m_includeFiles[incIndex].hFileMap = CreateFileMappingA(m_includeFiles[incIndex].hFile, NULL, PAGE_READONLY, m_includeFiles[incIndex].FileSize.HighPart, m_includeFiles[incIndex].FileSize.LowPart, pFileName);
			if (m_includeFiles[incIndex].hFileMap == NULL)
			{
				if (m_includeFiles[incIndex].hFile != INVALID_HANDLE_VALUE)
					CloseHandle(m_includeFiles[incIndex].hFile);
				return E_FAIL;
			}

			// Create Map view
			*ppData = MapViewOfFile(m_includeFiles[incIndex].hFileMap, FILE_MAP_READ, 0, 0, 0);
			*pBytes = m_includeFiles[incIndex].FileSize.LowPart;

			// Success - Increment the include file count
			m_nIncludes = incIndex;

			return S_OK;
		}

		STDMETHOD(Close(LPCVOID pData))
		{
			// Defer Closure until the container destructor 
			SetCurrentDirectoryW(filePath);
			aDBG(filePath);
			return S_OK;
		}
	};
}