#pragma once

#include"EffectClass.hpp"
#include"VHBlurClass.hpp"
#include"DepthMap.hpp"
#include<memory>

namespace aMazing
{
	class DepthField :public EffectClass
	{
	public:
		HRESULT initialize(ID3D11Device* device, ID3D11DeviceContext* context)
		{
			HRESULT hr;
			tex2d.reset(new FrameBuffer);
			hr = tex2d->initialize(device, context);
			if (FAILED(hr))
			{
				return hr;
			}

			blur.reset(new VHBlurClass);
			hr = blur->initialize(device, context);
			if (FAILED(hr))
			{
				return hr;
			}

			depthMap.reset(new DepthMap);
			hr = depthMap->initialize(device, context);
			if (FAILED(hr))
			{
				return hr;
			}

			hr = EffectClass::initialize(device, context);
			if (FAILED(hr))
			{
				return hr;
			}
			return S_OK;
		}

		void Render(ID3D11Device* device,
			ID3D11DeviceContext* context,
			std::function<void(ID3D11Device*, ID3D11DeviceContext*)> renderFunction)
		{
			tex2d->clearDepthBuffer(device, context);
			tex2d->clearRenderTarget(device, context);
			tex2d->setRenderTarget(device, context);
			renderFunction(device, context);

			depthMap->Render(device, context, renderFunction);
			blur->Render(device, context, 3, 3, renderFunction);

			clearDepthStencil(device, context);
			clearRenderTarget(device, context);
			setRenderTarget(device, context);
			SHADERS.bindPair("DepthFieldMerge", device, context);
			tex2d->bindPS(device, context, 0);
			blur->bindPS(device, context, 1);
			depthMap->bindPS(device, context, 2);
			GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
		}
	private:
		std::unique_ptr<FrameBuffer> tex2d;
		std::unique_ptr<DepthMap> depthMap;
		std::unique_ptr<VHBlurClass> blur;
	};
}
