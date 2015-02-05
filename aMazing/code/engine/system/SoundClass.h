#pragma once
#include<memory>
#include<Windows.h>
#include<mmsystem.h>
#include<dsound.h>
#include<vector>
#include<thread>
#include"MutableString.h"
#include"../../3rd-party/audiere/include/audiere.h"
using namespace audiere;
#pragma comment(lib, "3rd-party/audiere/lib/audiere.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

class SoundClass
{
public:
	SoundClass();
	~SoundClass();

	HRESULT Initialize(HWND hwnd, MutableString&& path);

	bool Play();
	bool Stop();
private:
	HWND m_hWnd;
	std::unique_ptr<OutputStreamPtr> m_soundData;
};

