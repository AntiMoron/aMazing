#pragma once

#include<D3D11.h>
#include<D3DX11.h>
#include<xnamath.h>
#include"WindowClass.h"
#include"Defines.hpp"

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
	HRESULT Shutdown();

	void setRenderTarget(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11DepthStencilView* depth);
	void clearRenderTarget(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11DepthStencilView* depth);

	void bindVS(ID3D11Device* device,
		ID3D11DeviceContext* context,
		unsigned int textureSlot);
	void bindPS(ID3D11Device* device,
		ID3D11DeviceContext* context,
		unsigned int textureSlot);

private:
	void clearDepthBuffer(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11DepthStencilView* depth);
	friend class EffectClass;
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
};