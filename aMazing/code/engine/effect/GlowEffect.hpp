#pragma once

#include<memory>
#include"EffectClass.hpp"
#include"VHBlurClass.hpp"
#include"HighlightExtractor.hpp"

namespace aMazing
{
	class GlowEffect :public EffectClass
	{
	public:
		HRESULT initialize(ID3D11Device* device, ID3D11DeviceContext* context)
		{
			HRESULT hr;

			blur.reset(new VHBlurClass);
			hr = blur->initialize(device, context);
			if (FAILED(hr))
			{
				return hr;
			}

			highlight.reset(new FrameBuffer);
			hr = highlight->initialize(device, context);
			if (FAILED(hr))
			{
				return hr;
			}


			tex2d.reset(new FrameBuffer);
			hr = tex2d->initialize(device, context);
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
			tex2d->clearRenderTarget(device, context);
			tex2d->setRenderTarget(device, context);

			renderFunction(device, context);

			highlight->setRenderTarget(device, context);
			highlight->clearRenderTarget(device, context);
			SHADERS.bindPair("HighLight", device, context);
			tex2d->bindPS(device, context, 0);
			GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);

			auto f1 = [&](ID3D11Device* device, ID3D11DeviceContext* context)
			{
				SHADERS.bindPair("Basic2D", device, context);
				highlight->bindPS(device, context, 0);
				GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
			};

			blur->Render(device, context, 3, 3, f1);

			setRenderTarget(device, context);
			clearRenderTarget(device, context);
			clearDepthStencil(device, context);

			SHADERS.bindPair("GlowMerge", device, context);
			tex2d->bindPS(device, context, 0);
			blur->bindPS(device, context, 1);
			GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
		}
	private:
		std::unique_ptr<FrameBuffer> tex2d;
		std::unique_ptr<FrameBuffer> highlight;
		std::unique_ptr<VHBlurClass> blur;
	};
}
