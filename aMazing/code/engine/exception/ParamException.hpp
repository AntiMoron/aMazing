#pragma once
/*
	Parameter error exception
*/
#include"CommonException.hpp"
#include<initializer_list>

namespace aMazing
{
	class ParamException : public CommonException
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
	};
}
