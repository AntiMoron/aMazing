#pragma once
#include<D3D11.h>
#include<D3DX11.h>
#include"EffectClass.h"

class DepthMap : public EffectClass
{
public:
	DepthMap()
	{
		;
	}
	~DepthMap()
	{
		;
	}

	template<typename... T>
	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		ID3D11DepthStencilView* depth,
		func_type_wrapper<std::function<void(ID3D11Device*, ID3D11DeviceContext*)> >::type renderFunction)
	{
		setRenderTarget(device, context);
		clearRenderTarget(device, context);
		SHADERS.getPair("DepthMap").bindShader(device, context);
		renderFunction(device, context);
	}
};