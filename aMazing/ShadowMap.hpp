#pragma once

#include"DepthMap.hpp"

class ShadowMap : public EffectClass
{
public:
	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context)
	{
		HRESULT hr;
		depthBuffer.reset(new DepthMap);
		hr = depthBuffer->Initialize(device, context);
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
		depthBuffer->Shutdown();
		EffectClass::Shutdown();
	}

	template<typename ...T>
	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		common_tool::functionType<std::function<void(ID3D11Device*, ID3D11DeviceContext*)> > renderFunction)
	{
		depthBuffer->Render(device, context, renderFunction);
		setRenderTarget(device, context);
		clearRenderTarget(device, context);
		depthBuffer->bindPS(device, context, 1);
		SHADERS.bindPair("ShadowMap", device, context);
		SHADERS.DisableShaderBind();
		renderFunction(device, context);
		SHADERS.EnableShaderBind();
	}
private:
	std::unique_ptr<DepthMap> depthBuffer;
};