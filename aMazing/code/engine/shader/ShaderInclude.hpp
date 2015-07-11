#pragma once
#include <Windows.h>
#include "../../common/CommonDxSupport.hpp"
#include "../system/file/FileSystem.hpp"
#include "../containers/string.hpp"

namespace aMazing
{
	class ShaderInclude : public ID3DInclude
	{
	private:
		const static unsigned int MAX_INCLUDES = 18U;
		struct sInclude
		{
			HANDLE         hFile;
			HANDLE         hFileMap;
			LARGE_INTEGER  FileSize;
			void           *pMapData;
		};

		struct sInclude   m_includeFiles[MAX_INCLUDES];
		unsigned int      m_nIncludes;

		void recursivelyOpen(const char* currentDir, const char* fileName)
		{
			aString fileContent(FileSystem::readFileA(fileName).c_str());
			aVector<aString> includePaths;
			const static aString indicatorStr = "#include";
			aString::size_type cnt = 0;
			aString::iterator pos = fileContent.findByIndex(cnt, indicatorStr);
			do{
				aString includePath = "";
				pos += indicatorStr.length();
				while (pos != fileContent.end() && isBlank(*pos))
					++pos;
				++pos;
				while (pos != fileContent.end() && !isBlank(*pos) && ((*pos) != '\"'))
				{
					includePath += *pos;
					++pos;
				}
				includePaths.push_back(includePath);
				++cnt;
				pos = fileContent.findByIndex(cnt, indicatorStr);
			} while (pos != fileContent.end());
			for (aString& includePath : includePaths)
			{
				std::string nextFilePath = cutFilePath(currentDir) + '/';
				nextFilePath += cutFilePath(includePath.c_str());
				SetCurrentDirectoryA(nextFilePath.c_str());
				;
				recursivelyOpen(nextFilePath.c_str(), fileName);
			}
			SetCurrentDirectoryA(currentDir);
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
			m_nIncludes = 0;
		}
		//get file directory from file path.
		std::string cutFilePath(const std::string& src)
		{
			int aindex = src.rfind('\\');
			int bindex = src.rfind('/');
			int index = (std::max) (aindex, bindex);
			if (index < 0)
			{
				return "";
			}
			return src.substr(0, index);
		}

		STDMETHOD(Open(
			D3D_INCLUDE_TYPE,
			LPCSTR pFileName,
			LPCVOID ,
			LPCVOID *ppData,
			UINT * pBytes
			))
		{
			char filePath[MAX_PATH];
			unsigned int   incIndex = m_nIncludes + 1;
			auto nBytes = GetCurrentDirectoryA(MAX_PATH, filePath);
			if (nBytes >= MAX_PATH)
			{
				return E_FAIL;
			}
			
			std::string cutedFilePath = cutFilePath(pFileName);
			SetCurrentDirectoryA(cutedFilePath.c_str());
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
			SetCurrentDirectoryA(filePath);
			return S_OK;
		}

		STDMETHOD(Close(LPCVOID))
		{
			// Defer Closure until the container destructor 
			return S_OK;
		}
	};
}