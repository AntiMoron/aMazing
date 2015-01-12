#pragma once

#include"EffectClass.h"

class HighlightExtractor : public EffectClass
{
public:
	HRESULT Initialize(ID3D11Device* device,ID3D11DeviceContext* context)
	{
		HRESULT hr;
		highlight.reset(new FrameBuffer);
		hr = highlight->Initialize(device, context);
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

	template<typename ...T>
	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		common_tool::functionType<std::function<void(ID3D11Device*, ID3D11DeviceContext*)> > renderFunction)
	{
		highlight->clearRenderTarget(device, context);
		highlight->setRenderTarget(device, context);
		
		renderFunction(device, context);

		setRenderTarget(device, context);
		clearRenderTarget(device, context);
		clearDepthStencil(device, context);
		SHADERS.bindPair("HighLight", device, context);
		highlight->bindPS(device, context, 0);
		GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
	}
private:
	std::unique_ptr<FrameBuffer> highlight;
};