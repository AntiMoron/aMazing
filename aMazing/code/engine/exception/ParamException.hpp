#pragma once
/*
	Parameter error exception
*/

#include<exception>
#include<string>
#include<initializer_list>
using std::exception;

namespace aMazing
{
	class ParamException : public exception
	{
	public:
		ParamException()
		{
			msg = "parameter has error(s)";
		}
		ParamException(std::initializer_list<std::string> multipleThing)
		{
			msg.clear();
			for (auto& val : multipleThing)
			{
				msg += val;
			}
		}
		const char* what() const
		{
			return msg.c_str();
		}
	private:
		std::string msg;
	};
}
