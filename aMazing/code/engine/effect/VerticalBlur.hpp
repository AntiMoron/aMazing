#pragma once
#include"EffectClass.hpp"

namespace aMazing
{
	class VerticalBlur : public EffectClass
	{
	public:
		VerticalBlur()
		{
			;
		}
		~VerticalBlur()
		{
			;
		}

		HRESULT initialize(ID3D11Device* device,
			ID3D11DeviceContext* context)
		{
			HRESULT hr;
			vertBuffer.reset(new FrameBuffer);
			hr = vertBuffer->initialize(device, context);
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
			vertBuffer->setRenderTarget(device, context);
			vertBuffer->clearRenderTarget(device, context);

			renderFunction(device, context);

			setRenderTarget(device, context);
			clearRenderTarget(device, context);
			vertBuffer->bindPS(device, context, 0);
			SHADERS.bindPair("Vblur", device, context);
			GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
		}
	private:
		std::unique_ptr<FrameBuffer> vertBuffer;
	};
}
