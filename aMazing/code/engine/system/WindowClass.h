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

	void setResolutionWidth(unsigned short width);
	void setResolutionHeight(unsigned short height);

	unsigned short getWidth();
	unsigned short getHeight();

	unsigned short getResolutionWidth();
	unsigned short getResolutionHeight();

	float getAspectRatio();
private:
	unsigned short wndWidth;
	unsigned short wndHeight;
	unsigned short resolutionWidth;
	unsigned short resolutionHeight;
};

#define WINWIDTH (WindowClass::getInstance().getWidth())
#define WINHEIGHT (WindowClass::getInstance().getHeight())
#define RESWIDTH (WindowClass::getInstance().getResolutionWidth())
#define RESHEIGHT (WindowClass::getInstance().getResolutionHeight())
#define ASPECTRATIO (WindowClass::getInstance().getAspectRatio())