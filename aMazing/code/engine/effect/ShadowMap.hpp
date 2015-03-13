#pragma once

#include"DepthMap.hpp"

namespace aMazing
{
	class ShadowMap : public EffectClass
	{
	public:
		HRESULT Initialize(ID3D11Device* device,
			ID3D11DeviceContext* context)
		{
			HRESULT hr;
			depthBuffer.reset(new FrameBuffer);
			hr = depthBuffer->Initialize(device, context, WINWIDTH, WINHEIGHT); 
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

		void Render(ID3D11Device* device,
			ID3D11DeviceContext* context,
			std::function<void(ID3D11Device*, ID3D11DeviceContext*)> renderFunction)
		{
			depthBuffer->setRenderTarget(device, context);
			depthBuffer->clearRenderTarget(device, context);
			SHADERS.bindPair("LightDepthMap", device, context);
			//SHADERS.bindPair("Basic3D", device, context);
			SHADERS.DisableShaderBind();
			renderFunction(device, context);
			SHADERS.EnableShaderBind();

			setRenderTarget(device, context);
			clearRenderTarget(device, context);
			clearDepthStencil(device, context);
//			#define _DB
#ifdef _DB
			depthBuffer->bindPS(device, context, 0);
			SHADERS.bindPair("Basic2D", device, context);
			GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
			return;
#else
			depthBuffer->bindPS(device, context, 1);
			SHADERS.bindPair("ShadowMap", device, context);
			SHADERS.DisableShaderBind();
			renderFunction(device, context);
			SHADERS.EnableShaderBind();
#endif
		}
	private:
		std::unique_ptr<FrameBuffer> depthBuffer;
	};
}
