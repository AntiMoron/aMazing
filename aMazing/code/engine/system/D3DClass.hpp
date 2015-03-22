#pragma once
#include "../../common/CommonDxSupport.hpp"
#include "WindowClass.hpp"
#include "Defines.hpp"
#include <chrono>

namespace aMazing
{
	class D3DClass
	{
	public:
		D3DClass();
		~D3DClass();

		HRESULT Initialize(HWND hwnd);

		ID3D11Device* getDevice();
		ID3D11DeviceContext* getContext();
		ID3D11RenderTargetView* getRenderTargetView();
		ID3D11DepthStencilView* getDepthStencilView();

		const bool isInited() const;
		void EnableDepth();
		void DisableDepth();

		void setRenderTarget();
		void clearRenderTarget();
		void clearDepthStencil();
		void Present(bool VSync);

		size_t getFps();
	private:
		volatile bool bIsInited;
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
		ID3D11SamplerState*                 g_pSamplerClamp;

		ID3D11DepthStencilState*			m_depthEnabledStencilState;
		ID3D11DepthStencilState*			m_depthDisabledStencilState;
		ID3D11BlendState*					AlphaRenderingEnabled;
		std::chrono::high_resolution_clock  clock;
		std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
		size_t								framePerSecond;
		size_t								frameCounter;
	};
}
