#pragma once
#include <fstream>
#include <string>
#include "../../../engine/Config.hpp"
#include "../../exception/FailureException.hpp"
#include "../../system/MutableString.hpp"

namespace aMazing
{
	class FileSystem
	{
	public:
		static std::basic_string<char> readFileA(const char* fileName) throw(FailureException)
		{
			std::ifstream fStream(fileName);
			if (!fStream.is_open())
			{
				throw FailureException("File read failed.");
			}
			return std::string(std::istreambuf_iterator<char>(fStream),
				std::istreambuf_iterator<char>());
		}
		static std::basic_string<char> readFileW(const wchar_t* fileName)
		{
			return readFileA(MutableString(fileName).getMultiByteString().c_str());
		}
	};
}
