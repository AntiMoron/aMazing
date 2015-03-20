#pragma once
#include"../system/PrimitivePipeline.hpp"
#include"../system/FrameBuffer.hpp"
#include"../shader/ShaderManager.hpp"
#include"../../common/CommonUtil.hpp"
#include<functional>
#include<memory>

namespace aMazing
{
	class EffectClass
	{
	public:
		EffectClass()
		{
			is_init = false;
		}
		virtual ~EffectClass(){}

		HRESULT Initialize(ID3D11Device* device,
			ID3D11DeviceContext* context)
		{
			HRESULT hr;
			fbo.reset(new FrameBuffer);
			hr = fbo->Initialize(device, context);
			if (FAILED(hr))
			{
				return hr;
			}
			is_init = true;
			return S_OK;
		}

		void clearRenderTarget(ID3D11Device* device,
			ID3D11DeviceContext* context)
		{
			fbo->clearRenderTarget(device, context);
		}

		void setRenderTarget(ID3D11Device* device,
			ID3D11DeviceContext* context)
		{
			fbo->setRenderTarget(device, context);
		}

		void bindVS(ID3D11Device* device,
			ID3D11DeviceContext* context,
			unsigned int textureSlot)
		{
			fbo->bindVS(device, context, textureSlot);
		}

		void bindPS(ID3D11Device* device,
			ID3D11DeviceContext* context,
			unsigned int textureSlot)
		{
			fbo->bindPS(device, context, textureSlot);
		}

		void clearDepthStencil(ID3D11Device* device,
			ID3D11DeviceContext* context)
		{
			fbo->clearDepthBuffer(device, context);
		}

		const bool isInited()const
		{
			return is_init;
		}

		//void Render
		//Call the renderFunction function in argument-list to render Objects.
	protected:
		std::unique_ptr<FrameBuffer> fbo;
		volatile bool is_init;
	};
}
