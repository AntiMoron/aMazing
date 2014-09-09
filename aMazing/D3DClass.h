#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include "WindowClass.h"
#include "Defines.hpp"

class D3DClass
{
public:
	D3DClass();
	~D3DClass();

	HRESULT Initialize(HWND hwnd);
	HRESULT Shutdown();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getContext();
	ID3D11RenderTargetView* getRenderTargetView();
	ID3D11DepthStencilView* getDepthStencilView();

	void EnableDepth();
	void DisableDepth();

	void setRenderTarget();
	void clearDepthStencil();
	void Present(bool VSync);
private:
	D3D_DRIVER_TYPE                     g_driverType;
	D3D_FEATURE_LEVEL                   g_featureLevel;
	ID3D11Device*                       g_pd3dDevice;
	ID3D11DeviceContext*                g_pImmediateContext;
	IDXGISwapChain*                     g_pSwapChain;
	ID3D11RenderTargetView*             g_pRenderTargetView;
	ID3D11Texture2D*                    g_pDepthStencil;
	ID3D11DepthStencilView*             g_pDepthStencilView;
	ID3D11ShaderResourceView*           g_pTextureRV;
	ID3D11SamplerState*                 g_pSamplerLinear;

	ID3D11DepthStencilState*			m_depthEnabledStencilState;
	ID3D11DepthStencilState*			m_depthDisabledStencilState;
};

