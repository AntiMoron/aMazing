#pragma once
#include<Windows.h>
#include<mmsystem.h>
#include<dsound.h>
#include<vector>
#include<thread>
#include"MutableString.h"
#include"audiere/include/audiere.h"
using namespace audiere;
#pragma comment(lib, "audiere/lib/audiere.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

class SoundClass
{
public:
	SoundClass();
	~SoundClass();

	HRESULT Initialize(HWND hwnd, MutableString&& path);
	HRESULT Shutdown();

	bool Play();
	bool Stop();
private:
	HWND m_hWnd;
	OutputStreamPtr* m_soundData;
	AudioDevicePtr* m_audioDevice;
};

