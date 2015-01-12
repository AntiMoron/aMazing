#pragma once
/*
	Parameter error exception
*/

#include<exception>
#include<string>
using std::exception;

class ParamException : public exception
{
public:
	ParamException()
	{
		msg = "parameter has error(s)";
	}
	const char* what() const
	{
		return msg.c_str();
	}
private:
	std::string msg;
};