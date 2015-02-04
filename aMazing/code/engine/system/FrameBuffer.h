#pragma once

#include"../../common/CommonDxSupport.hpp"
#include"WindowClass.h"
#include"Defines.hpp"

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context,unsigned short imageWidth,unsigned short imageHeight);

	HRESULT EnableMultiSampling();
	HRESULT DisableMultiSampling();

	void setRenderTarget(ID3D11Device* device, ID3D11DeviceContext* context);
	void clearRenderTarget(ID3D11Device* device, ID3D11DeviceContext* context);

	void bindVS(ID3D11Device* device,
		ID3D11DeviceContext* context,
		unsigned int textureSlot);
	void bindPS(ID3D11Device* device,
		ID3D11DeviceContext* context,
		unsigned int textureSlot);

	bool isInited()const;
	void clearDepthBuffer(ID3D11Device* device, ID3D11DeviceContext* context);
private:
	friend class EffectClass;
	volatile bool is_inited;
	bool multiSampling;
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11Texture2D* m_renderTargetTextureMS;//Multi-sampled
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
	ID3D11Texture2D* m_pDepthStencil;
	ID3D11DepthStencilView* m_depthStencilView;
};