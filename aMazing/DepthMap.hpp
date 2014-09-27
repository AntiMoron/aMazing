#pragma once
#include<D3D11.h>
#include<D3DX11.h>
#include"EffectClass.h"
#include"CameraClass.h"

class DepthMap : public EffectClass
{
public:
	template<typename ...T>
	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		common_tool::functionType<std::function<void(ID3D11Device*, ID3D11DeviceContext*)> > renderFunction)
	{
		clearRenderTarget(device, context);
		clearDepthStencil(device, context);
		setRenderTarget(device, context);
		SHADERS.bindPair("DepthMap", device, context);
		SHADERS.DisableShaderBind();
		renderFunction(device, context);
		SHADERS.EnableShaderBind();
	}
};