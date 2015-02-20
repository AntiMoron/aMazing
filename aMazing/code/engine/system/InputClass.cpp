#include "InputClass.hpp"
using namespace aMazing;
InputClass* InputClass::instance = nullptr;

InputClass& InputClass::GetInst()
{
	if (instance == nullptr)
	{
		instance = new InputClass;
	}
	return *instance;
}

InputClass::InputClass()
{
	mouse_x = 0;
	mouse_y = 0;
	//set all the keys false.
	for (auto& param : keys)
	{
		param = false;
	}
}


InputClass::~InputClass()
{
	;
}