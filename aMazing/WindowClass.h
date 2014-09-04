#pragma once

#include<D3D11.h>

class WindowClass
{
public:
	WindowClass();
	~WindowClass();

	static WindowClass& getInstance();

	void setWidth(unsigned short width);
	void setHeight(unsigned short width);

	unsigned short getWidth();
	unsigned short getHeight();

	float getAspectRatio();
private:
	unsigned short wnd_width;
	unsigned short wnd_height;
	static WindowClass* wnd_instance;
};

#define WINWIDTH (WindowClass::getInstance().getWidth())
#define WINHEIGHT (WindowClass::getInstance().getHeight())
#define ASPECTRATIO (WindowClass::getInstance().getAspectRatio())