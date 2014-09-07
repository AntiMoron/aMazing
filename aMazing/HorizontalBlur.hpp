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
	
	template<typename... T>
	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		ID3D11DepthStencilView* depth,
		func_type_wrapper<std::function<void(ID3D11Device*, ID3D11DeviceContext*)> >::type renderFunction )
	{
		setRenderTarget(device, context, depth);
		renderFunction(device, context);
	}
};
