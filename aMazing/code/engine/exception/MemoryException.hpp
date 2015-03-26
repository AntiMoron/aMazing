#pragma once
#include"CommonException.hpp"

namespace aMazing
{
	class MemoryException :public CommonException
	{
	public:
		MemoryException()
		{
			msg = "Memory Access Error.";
		}
		MemoryException(const char* what)
		{
			msg = what;
		}
	};
}
