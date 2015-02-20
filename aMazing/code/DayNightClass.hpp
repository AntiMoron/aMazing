#pragma once
#include<cmath>
#include<ctime>
#include<chrono>
#include<Windows.h>
#include<xnamath.h>
#include<memory>
#include"engine/system/GPUConstantBuffer.hpp"
#include"engine/system/AmbientLight.hpp"
#define TIME_DAY 240

class DayNightClass
{
public:
	DayNightClass()
	{
		timeNow = rand() % TIME_DAY;
		lastTick = GetTickCount();
	}

	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		HRESULT hr;
		dayColorAlpha.reset(new GPUConstantBuffer<XMFLOAT4>);
		hr = dayColorAlpha->Initialize(device, context, 4);
		if (FAILED(hr))
		{
			return hr;
		}
		ambientLight.reset(new AmbientLight);
		hr = ambientLight->Initialize(device, context);
		if (FAILED(hr))
		{
			return hr;
		}
		return S_OK;
	}
	void UpdateTime(ID3D11Device* device,ID3D11DeviceContext* context)
	{
		std::size_t t = GetTickCount();
		if (t - lastTick >= 1000)
		{
			timeNow += (t - lastTick) / 1000;
			timeNow %= TIME_DAY;
			lastTick = t;
			float tn = timeNow;
			if (timeNow > (TIME_DAY / 2.0f))
			{
				tn = TIME_DAY - timeNow;
			}
			XMFLOAT4 data = { 2.0f * float(tn) / TIME_DAY,
				2.0f * float(tn) / TIME_DAY,
				2.0f * float(tn) / TIME_DAY,
				1.0f };
			
			ambientLight->setPosition(XMFLOAT3(10.0f * std::sin(float(timeNow) / TIME_DAY * 360.0f * 0.017453292f),
				10.0f,
				10.0f * std::cos(float(timeNow) / TIME_DAY * 360.0f * 0.017453292f)));
			ambientLight->setTarget(XMFLOAT3(0.0f, 0.0f, 0.0f));
			ambientLight->Render(device, context);

			dayColorAlpha->UpdateData(&data);
			dayColorAlpha->UpdateGpu(device, context);
			dayColorAlpha->BindPixelShader(device, context);
		}
	}

	//whether it's day or not
	bool isDay() const
	{
		return (timeNow >= (0.375*TIME_DAY) && 
			(timeNow <= (0.875 * TIME_DAY)));
	}
private:
	std::size_t lastTick;
	std::size_t timeNow;
	std::unique_ptr<GPUConstantBuffer<XMFLOAT4> > dayColorAlpha;
	std::unique_ptr<AmbientLight> ambientLight;
};
