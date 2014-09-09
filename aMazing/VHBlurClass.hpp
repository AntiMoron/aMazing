#pragma once
#include"HorizontalBlur.hpp"
#include"VerticalBlur.hpp"
#include"RectangleClass.h"
#include<memory>

class VHBlurClass : public EffectClass
{
public:
	VHBlurClass(){}
	~VHBlurClass(){}

	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context)
	{
		HRESULT hr;
		m_buffer.reset(new FrameBuffer);
		hr = m_buffer->Initialize(device, context);
		if (FAILED(hr))
		{
			return hr;
		}

		hr = EffectClass::Initialize(device, context);		//must last
		if (FAILED(hr))
		{
			return hr;
		}
		return S_OK;
	}

	HRESULT Shutdown()
	{
		m_buffer->Shutdown();
		EffectClass::Shutdown();
		return S_OK;
	}

	template<typename... T>
	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		ID3D11DepthStencilView* depth,
		unsigned short hBlurCount,
		unsigned short vBlurCount,
		func_type_wrapper<std::function<void(ID3D11Device*, ID3D11DeviceContext*)> >::type renderFunction)
	{
		fbo->setRenderTarget(device, context, depth);
		fbo->clearRenderTarget(device, context, depth);
		renderFunction(device, context);
		fbo->bindPS(device, context, 0);

		m_buffer->setRenderTarget(device, context, depth);
		m_buffer->clearRenderTarget(device, context, depth);
		SHADERS.getPair("Hblur").bindShader(device, context);
		GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);

		fbo->clearRenderTarget(device, context, depth);
		fbo->setRenderTarget(device, context, depth);
		m_buffer->bindPS(device, context, 0);
		SHADERS.getPair("Vblur").bindShader(device, context);
		GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
		fbo->bindPS(device, context, 0);
	}
private:
	std::unique_ptr<FrameBuffer> m_buffer;
};

