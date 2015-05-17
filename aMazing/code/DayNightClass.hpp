#pragma once
#include<cmath>
#include<chrono>
#include<memory>
#include"engine/system/GPUConstantBuffer.hpp"
#define TIME_DAY 240

class DayNightClass
{
public:
	DayNightClass()
	{
		timeNow = rand() % TIME_DAY;
		lastTick = GetTickCount();
	}

	HRESULT initialize(ID3D11Device* device)
	{
		HRESULT hr;
		dayColorAlpha.reset(new GPUConstantBuffer<XMFLOAT4>);
		hr = dayColorAlpha->initialize(device, 4);
		if (FAILED(hr))
		{
			return hr;
		}
		return S_OK;
	}
	void UpdateTime(ID3D11DeviceContext* context)
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
				tn = float(TIME_DAY - timeNow);
			}
			XMFLOAT4 data = { 2.0f * float(tn) / TIME_DAY,
				2.0f * float(tn) / TIME_DAY,
				2.0f * float(tn) / TIME_DAY,
				1.0f };
			
			//ambientLight->setPosition(XMFLOAT3(5.0f * std::sin(float(timeNow) / TIME_DAY * 360.0f * 0.017453292f),
			//	5.0f,
			//	5.0f * std::cos(float(timeNow) / TIME_DAY * 360.0f * 0.017453292f)));
			//ambientLight->setTarget(XMFLOAT3(0.0f, 0.0f, 0.0f));
			//ambientLight->Render(device, context);

			dayColorAlpha->updateData(&data);
			dayColorAlpha->updateGpu(context);
			dayColorAlpha->bindPixelShader(context);
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
};
