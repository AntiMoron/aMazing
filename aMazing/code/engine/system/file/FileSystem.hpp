#pragma once
#include <fstream>
#include <string>
#include "../../../engine/Config.hpp"
#include "../../exception/FailureException.hpp"
#include "../../containers/MutableString.hpp"

namespace aMazing
{
	class FileSystem
	{
	public:
		static std::string readFileA(const char* fileName) throw(FailureException)
		{
			std::ifstream fStream(fileName);
			if (!fStream.is_open())
			{
				throw FailureException("File read failed.");
			}
			return std::string(std::istreambuf_iterator<char>(fStream),
				std::istreambuf_iterator<char>());
		}
		static std::string readFileW(const wchar_t* fileName)
		{
			return readFileA(MutableString(fileName).getMultiByteString().c_str());
		}
	};
}
