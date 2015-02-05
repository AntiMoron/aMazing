#include "SoundClass.h"

SoundClass::SoundClass(){}
SoundClass::~SoundClass(){}


HRESULT SoundClass::Initialize(HWND hwnd , MutableString&& path)
{
	HRESULT hr = S_OK;
	m_hWnd = hwnd;
	AudioDevicePtr audioDevice(OpenDevice());
	OutputStream* m_soundDataStream = OpenSound(audioDevice, path.getMultiByteString().c_str(), false);
	m_soundData.reset(new OutputStreamPtr(m_soundDataStream));
	if (m_soundData == nullptr ||
		m_soundData->get() == nullptr)
	{
		printf("No Sound File Found.\r\n");
		return E_FAIL;
	}
	(*m_soundData)->stop();
	return hr;
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