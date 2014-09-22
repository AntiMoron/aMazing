#pragma once
#include"EffectClass.h"

class HorizontalBlur : public EffectClass
{
public:
	HorizontalBlur()
	{
		;
	}
	~HorizontalBlur()
	{
		;
	}
	
	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context)
	{
		HRESULT hr; 
		horiBuffer.reset(new FrameBuffer);
		hr = horiBuffer->Initialize(device, context);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = EffectClass::Initialize(device, context);
		if (FAILED(hr))
		{
			return hr;
		}
		return S_OK;
	}

	void Shutdown()
	{
		horiBuffer->Shutdown();
		EffectClass::Shutdown();
	}

	template<typename... T>
	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		func_type_wrapper<std::function<void(ID3D11Device*, ID3D11DeviceContext*)> >::type renderFunction)
	{
		horiBuffer->setRenderTarget(device, context);
		horiBuffer->clearRenderTarget(device, context);

		renderFunction(device, context);

		setRenderTarget(device, context);
		clearRenderTarget(device, context);
		horiBuffer->bindPS(device, context, 0);
		SHADERS.getPair("Hblur").bindShader(device, context);
		GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
	}
private:
	std::unique_ptr<FrameBuffer> horiBuffer;
};