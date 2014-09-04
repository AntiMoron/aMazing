#include "WindowClass.h"

WindowClass* WindowClass::wnd_instance = nullptr;

WindowClass::WindowClass()
{
}


WindowClass::~WindowClass()
{
}

WindowClass& WindowClass::getInstance()
{
	if (wnd_instance == nullptr)
	{
		wnd_instance = new WindowClass;
	}
	return *wnd_instance;
}


void WindowClass::setWidth(unsigned short width)
{
	wnd_width = width;
}

void WindowClass::setHeight(unsigned short height)
{
	wnd_height = height;
}

unsigned short WindowClass::getWidth()
{
	return wnd_width;
}

unsigned short WindowClass::getHeight()
{
	return wnd_height;
}

float WindowClass::getAspectRatio()
{
	float result = 4 / 3.0f;
	if (wnd_width == wnd_height)
	{
		return 1.0f;
	}

	if (wnd_width > wnd_height)
	{
		if (wnd_height == 0)
		{
			wnd_height = wnd_width;
		}
		result = wnd_width / wnd_height;
	}
	else
	{
		if (wnd_width == 0)
		{
			wnd_width = wnd_height;
		}
		result = wnd_height / wnd_width;
	}
	return result;
}