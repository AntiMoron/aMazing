#pragma once
#include"CommonException.hpp"

namespace aMazing
{
	class FailureException : public CommonException
	{
	public:
		FailureException()
		{
			msg = "Failure Exception";
		}
		FailureException(const char* message)
		{
			msg = message;
		}
	};
}