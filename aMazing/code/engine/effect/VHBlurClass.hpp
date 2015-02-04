#pragma once
#include"HorizontalBlur.hpp"
#include"VerticalBlur.hpp"
#include<memory>

class VHBlurClass : public EffectClass
{
public:
	VHBlurClass(){}
	~VHBlurClass(){}

	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context)
	{
		HRESULT hr;
		m_vbuffer.reset(new FrameBuffer);
		m_hbuffer.reset(new FrameBuffer);
		hr = m_vbuffer->Initialize(device, context);
		if (FAILED(hr))
		{
			return hr;
		}

		hr = m_hbuffer->Initialize(device, context);
		if (FAILED(hr))
		{
			return hr;
		}

		hr = EffectClass::Initialize(device, context);		//must last
		if (FAILED(hr))
		{
			return hr;
		}
		return S_OK;
	}

	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		unsigned short hBlurCount,
		unsigned short vBlurCount,
		std::function<void(ID3D11Device*, ID3D11DeviceContext*)> renderFunction)
	{
		m_hbuffer->setRenderTarget(device, context);
		m_hbuffer->clearRenderTarget(device, context);
		
		renderFunction(device, context);
		
		auto pairMin = hBlurCount;
		if (pairMin > vBlurCount)
			pairMin = vBlurCount;
		bool isD = (pairMin & 0x1);
		for (unsigned short times = 0; times < pairMin; times++)
		{
			m_vbuffer->setRenderTarget(device, context);
			m_vbuffer->clearRenderTarget(device, context);
			m_hbuffer->bindPS(device, context, 0);
			SHADERS.bindPair("Vblur", device, context);
			GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);

			m_hbuffer->clearRenderTarget(device, context);
			m_hbuffer->setRenderTarget(device, context);
			m_vbuffer->bindPS(device, context, 0);
			SHADERS.bindPair("Hblur", device, context);
			GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
		}
		if (vBlurCount == hBlurCount)
		{
			setRenderTarget(device, context);
			clearRenderTarget(device, context);
			m_hbuffer->bindPS(device, context, 0);
			SHADERS.bindPair("Basic2D", device, context);
			GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
		}


		if(hBlurCount > pairMin)
		{
			hBlurCount -= pairMin;
			for (unsigned short times = 0; times < hBlurCount; times++)
			{
				if (times & 0x1)
				{
					m_hbuffer->setRenderTarget(device, context);
					m_hbuffer->clearRenderTarget(device, context);
					bindPS(device, context, 0);
					SHADERS.bindPair("Hblur", device, context);
					GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
				}
				else
				{
					setRenderTarget(device, context);
					clearRenderTarget(device, context);
					m_hbuffer->bindPS(device, context, 0);
					SHADERS.bindPair("Hblur", device, context);
					GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
				}
			}
			if (1 == (hBlurCount & 0x1))
				return;
			setRenderTarget(device, context);
			clearRenderTarget(device, context);
			m_hbuffer->bindPS(device, context, 0);
			SHADERS.bindPair("Hblur", device, context);
			GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
		}
		else if (vBlurCount > pairMin)
		{
			vBlurCount -= pairMin;
			for (unsigned short times = 0; times < vBlurCount; times++)
			{
				if (times & 0x1)
				{
					m_hbuffer->setRenderTarget(device, context);
					m_hbuffer->clearRenderTarget(device, context);
					bindPS(device, context, 0);
					SHADERS.bindPair("Vblur", device, context);
					GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
				}
				else
				{
					setRenderTarget(device, context);
					clearRenderTarget(device, context);
					m_hbuffer->bindPS(device, context, 0);
					SHADERS.bindPair("Vblur", device, context);
					GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
				}
			}
			if (1 == (vBlurCount & 0x1))
				return;
			setRenderTarget(device, context);
			clearRenderTarget(device, context);
			m_hbuffer->bindPS(device, context, 0);
			SHADERS.bindPair("Vblur", device, context);
			GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
		}
	}
private:
	std::unique_ptr<FrameBuffer> m_vbuffer;
	std::unique_ptr<FrameBuffer> m_hbuffer;
};

