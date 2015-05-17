#pragma once
#include<memory>
#include<Windows.h>
#include<mmsystem.h>
#include<dsound.h>
#include<vector>
#include<thread>
#include"../containers/MutableString.hpp"
#include"../../3rd-party/audiere/include/audiere.h"
using namespace audiere;
#pragma comment(lib, "3rd-party/audiere/lib/audiere.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
namespace aMazing
{
	class SoundClass
	{
	public:
		SoundClass();
		~SoundClass();

		HRESULT initialize(HWND hwnd, MutableString&& path);

		bool Play();
		bool Stop();
	private:
		HWND m_hWnd;
		OutputStreamPtr m_soundData;
	};
}
