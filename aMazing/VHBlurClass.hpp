#pragma once
#include"HorizontalBlur.hpp"
#include"VerticalBlur.hpp"
#include"RectangleClass.h"

class VHBlurClass : public EffectClass
{
public:
	VHBlurClass(){}
	~VHBlurClass(){}

	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context)
	{
		HRESULT hr;
		hr = vb.Initialize(device,context);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = hb.Initialize(device, context);
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
		EffectClass::Shutdown();
		vb.Shutdown();
		hb.Shutdown();
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
		setRenderTarget(device, context, depth);
		clearRenderTarget(device, context, depth);
		
		renderFunction(device, context);
		
		return;
		fbo.bindPS(device, context, 0);
		vb.setRenderTarget(device, context, depth);
		vb.clearRenderTarget(device, context, depth);
		SHADERS.getPair("Vblur").bindShader(device, context);
		GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);

		setRenderTarget(device, context, depth);
		clearRenderTarget(device, context, depth);
		vb.bindPS(device, context, 0);
		SHADERS.getPair("Hblur").bindShader(device, context);
		GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
	}
private:
	VerticalBlur vb;
	HorizontalBlur hb;
};

