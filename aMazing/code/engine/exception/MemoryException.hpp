#pragma once
#include<exception>
#include<string>
using std::exception;

namespace aMazing
{
	class MemoryException :public exception
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

		const char* what() const
		{
			return msg.c_str();
		}
	private:
		std::string msg;
	};
}
