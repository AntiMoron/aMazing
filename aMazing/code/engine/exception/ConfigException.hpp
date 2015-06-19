#pragma once
#include"CommonException.hpp"

namespace aMazing
{
	class ConfigException :public CommonException
	{
	public:
		ConfigException()
		{
			msg = "Config fail.";
		}
		ConfigException(const char* message)
		{
			msg = message;
		}
	};
}
