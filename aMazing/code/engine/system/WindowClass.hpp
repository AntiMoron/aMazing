#pragma once

#include<D3D11.h>

namespace aMazing
{
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
}

#define WINWIDTH (aMazing::WindowClass::getInstance().getWidth())
#define WINHEIGHT (aMazing::WindowClass::getInstance().getHeight())
#define RESWIDTH (aMazing::WindowClass::getInstance().getResolutionWidth())
#define RESHEIGHT (aMazing::WindowClass::getInstance().getResolutionHeight())
#define ASPECTRATIO (aMazing::WindowClass::getInstance().getAspectRatio())