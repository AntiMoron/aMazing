#pragma once
#include<exception>
#include<string>
using std::exception;

namespace aMazing
{
	class CommonException :public exception
	{
	public:
		CommonException()
		{
			msg = "Memory Access Error.";
		}
		CommonException(const char* what)
		{
			msg = what;
		}

		const char* what() const
		{
			return msg.c_str();
		}
	protected:
		std::string msg;
	};
}
