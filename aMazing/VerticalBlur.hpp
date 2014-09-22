#pragma once
#include"EffectClass.h"

class VerticalBlur : public EffectClass
{
public:
	VerticalBlur()
	{
		;
	}
	~VerticalBlur()
	{
		;
	}

	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context)
	{
		HRESULT hr;
		vertBuffer.reset(new FrameBuffer);
		hr = vertBuffer->Initialize(device, context);
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

	template<typename... T>
	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		func_type_wrapper<std::function<void(ID3D11Device*, ID3D11DeviceContext*)> >::type renderFunction)
	{
		vertBuffer->setRenderTarget(device, context);
		vertBuffer->clearRenderTarget(device, context);
		
		renderFunction(device, context);

		setRenderTarget(device, context);
		clearRenderTarget(device, context);
		vertBuffer->bindPS(device, context, 0);
		SHADERS.getPair("Vblur").bindShader(device, context);
		GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
	}
private:
	std::unique_ptr<FrameBuffer> vertBuffer;
};
