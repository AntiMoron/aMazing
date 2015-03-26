#pragma once
/*
	Parameter error exception
*/
#include"CommonException.hpp"
#include<initializer_list>

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
