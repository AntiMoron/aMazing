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
		hr = EffectClass::Initialize(device, context);
		if (FAILED(hr))
		{
			return hr;
		}
		is_init = false;
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
		//Unfinished.

		rec.Initialize(device, context);

		is_init = true;
		return S_OK;
	}

	HRESULT Shutdown()
	{
		EffectClass::Shutdown();
		vb.Shutdown();
		hb.Shutdown();
		return S_OK;
	}
	
	;

	template<typename... T>
	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		ID3D11DepthStencilView* depth,
		func_type_wrapper<std::function<void(ID3D11Device*, ID3D11DeviceContext*)> >::type renderFunction)
	{
		setRenderTarget(device, context, depth);

		hb.Render(device,
			context,
			depth,
			renderFunction);
		hb.bindPS(device, context, 0);

		auto renderFunction2 = [&](ID3D11Device* device,
			ID3D11DeviceContext* context)->void{
			SHADERS.getPair("Hblur").bindShader(device, context);
			hb.bindPS(device, context, 0);
			rec.Render(device, context, 0, 0, WINWIDTH, WINHEIGHT);
		};

		vb.setRenderTarget(d3d.getDevice(), d3d.getContext(), d3d.getDepthStencilView());
		vb.Render(device,
			context,
			depth,
			renderFunction2);

		fbo.setRenderTarget(device, context, depth);
		SHADERS.getPair("Vblur").bindShader(d3d.getDevice(), d3d.getContext());
		vb.bindPS(device, context, 0);
		rec.Render(device, context, 0, 0, WINWIDTH, WINHEIGHT);
	}
private:
	VerticalBlur vb;
	HorizontalBlur hb;
	RectangleClass rec;
};

