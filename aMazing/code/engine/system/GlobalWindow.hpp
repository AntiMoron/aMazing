#pragma once
#include "../../common/CommonDxSupport.hpp"
#include "../../common/CommonDef.hpp"
#include "../system/thread/aThreadSafeSingleton.hpp"
namespace aMazing
{
	class GlobalWindow : public aThreadSafeSingleton<GlobalWindow>
	{
	private:
		friend class aThreadSafeSingleton<GlobalWindow>;
	public:
		void setWindowHandler(HWND hwnd) aNOEXCEPT
		{
			gHwnd = hwnd;
		}
		HWND getWindowHandler() const aNOEXCEPT
		{
			return gHwnd;
		}

		void setWidth(unsigned short width) aNOEXCEPT
		{
			wndWidth = width;
		}
		void setHeight(unsigned short height) aNOEXCEPT
		{
			wndHeight = height;
		}

		void setResolutionWidth(unsigned short width) aNOEXCEPT
		{
			resolutionWidth = width;
		}
		void setResolutionHeight(unsigned short height) aNOEXCEPT
		{
			resolutionHeight = height;
		}

		unsigned short getWidth() const aNOEXCEPT
		{
			return wndWidth;
		}
		unsigned short getHeight() const aNOEXCEPT
		{
			return wndHeight;
		}

		unsigned short getResolutionWidth() const aNOEXCEPT
		{
			return resolutionWidth;
		}
		unsigned short getResolutionHeight() const aNOEXCEPT
		{
			return resolutionHeight;
		}

		float getAspectRatio() const aNOEXCEPT
		{
			float result = 16 / 9.0f;
			float tempWndWidth = wndWidth;
			float tempWndHeight = wndHeight;
			if (tempWndWidth == tempWndHeight)
			{
				return 1.0f;
			}

			if (tempWndWidth > tempWndHeight)
			{
				if (tempWndHeight == 0)
				{
					tempWndHeight = tempWndWidth;
				}
				result = float(tempWndWidth) / tempWndHeight;
			}
			else
			{
				if (tempWndWidth == 0)
				{
					tempWndWidth = tempWndHeight;
				}
				result = float(tempWndHeight) / tempWndWidth;
			}
			return result;
		}
	private:
		GlobalWindow(){}
		unsigned short wndWidth;
		unsigned short wndHeight;
		unsigned short resolutionWidth;
		unsigned short resolutionHeight;
		HWND gHwnd;
	};
}
#define GLOBAL_WINDOW (aMazing::GlobalWindow::getInstance())
#define WINWIDTH (aMazing::GlobalWindow::getInstance().getWidth())
#define WINHEIGHT (aMazing::GlobalWindow::getInstance().getHeight())
#define RESWIDTH (aMazing::GlobalWindow::getInstance().getResolutionWidth())
#define RESHEIGHT (aMazing::GlobalWindow::getInstance().getResolutionHeight())
#define ASPECTRATIO (aMazing::GlobalWindow::getInstance().getAspectRatio())
