#include "SoundClass.h"

SoundClass::SoundClass()
{
	m_audioDevice = nullptr;
}


SoundClass::~SoundClass()
{
}


HRESULT SoundClass::Initialize(HWND hwnd , MutableString&& path)
{
	HRESULT hr = S_OK;
	m_hWnd = hwnd;

	//Read Sound File right now;
	m_audioDevice = new AudioDevicePtr(OpenDevice());
	if (m_audioDevice == nullptr)
	{
		printf("No device\r\n");
		return E_FAIL;
	}
	OutputStream* m_soundDataStream = OpenSound(*m_audioDevice, path.getMultiByteString().c_str(), false);
	m_soundData = new OutputStreamPtr(m_soundDataStream);
	if (m_soundData == nullptr)
	{
		printf("No Sound File Found.\r\n");
		return E_FAIL;
	}
	(*m_soundData)->stop();
	return hr;
}

HRESULT SoundClass::Shutdown()
{
	if (m_soundData != nullptr)
	{
		delete m_soundData;
		m_soundData = nullptr;
	}
	return S_OK;
}

bool SoundClass::Play()
{
	(*m_soundData)->play();
	return true;
}

bool SoundClass::Stop()
{
	(*m_soundData)->stop();
	return true;
}