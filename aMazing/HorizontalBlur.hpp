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
		result.reset(new FrameBuffer);
		result->Initialize(device, context);
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

	template<typename... T>
	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		ID3D11DepthStencilView* depth,
		func_type_wrapper<std::function<void(ID3D11Device*, ID3D11DeviceContext*)> >::type renderFunction)
	{

		horiBuffer->setRenderTarget(device, context);
		horiBuffer->clearRenderTarget(device, context);

		renderFunction(device, context);
		horiBuffer->bindPS(device, context, 0);

		result->setRenderTarget(device, context);
		result->clearRenderTarget(device, context);
		SHADERS.getPair("Hblur").bindShader(device, context);
		GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
	}

	void bindPS(ID3D11Device* device, ID3D11DeviceContext* context,unsigned int textureSlot)
	{
		result->bindPS(device, context, textureSlot);
	}
private:
	std::unique_ptr<FrameBuffer> horiBuffer;
	std::unique_ptr<FrameBuffer> result;
};