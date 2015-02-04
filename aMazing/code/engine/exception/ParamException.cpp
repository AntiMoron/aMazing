#pragma once
/*
	Parameter error exception
*/

#include<exception>
#include<string>
#include<initializer_list>
using std::exception;

class ParamException : public exception
{
public:
	ParamException()
	{
		msg = "parameter has error(s)";
	}
	ParamException(std::initializer_list<std::string> multipleThing)
	{
		;
	}
	const char* what() const
	{
		return msg.c_str();
	}
private:
	std::string msg;
};