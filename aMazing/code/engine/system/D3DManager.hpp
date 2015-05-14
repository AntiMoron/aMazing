#pragma once
#include"../../common/CommonDxSupport.hpp"
#include"../../common/CommonDef.hpp"
#include"GlobalWindow.hpp"
#include"ManagedTypes.hpp"
#include<unordered_map>
#include<chrono>

namespace aMazing
{
	class D3DManager
	{
	public:
		D3DManager() = delete;

		static HRESULT Initialize(HWND hwnd)
		{
			HRESULT hr = E_FAIL;
			lastTime = clock.now();
			UINT createDeviceFlags = 0;
#ifdef _DEBUG
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
			D3D_DRIVER_TYPE driverTypes[] =
			{
				D3D_DRIVER_TYPE_HARDWARE,
				D3D_DRIVER_TYPE_WARP,
				D3D_DRIVER_TYPE_REFERENCE,
				D3D_DRIVER_TYPE_SOFTWARE,
			};
			UINT numDriverTypes = ARRAYSIZE(driverTypes);
			D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
			};
			UINT numFeatureLevels = ARRAYSIZE(featureLevels);
			DXGI_SWAP_CHAIN_DESC sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = 1;
			sd.BufferDesc.Width = WINWIDTH;
			sd.BufferDesc.Height = WINHEIGHT;
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = hwnd;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.Windowed = TRUE;

			for (unsigned int driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
			{
				driverType = driverTypes[driverTypeIndex];
				hr = D3D11CreateDeviceAndSwapChain(nullptr,
					driverType, nullptr, createDeviceFlags,
					featureLevels, numFeatureLevels,
					D3D11_SDK_VERSION, &sd, &pSwapChain,
					&pDefaultDevice, &featureLevel, &pImmediateContext);
				if (SUCCEEDED(hr))
					break;
			}
			if (FAILED(hr))
			{
				return hr;
			}
			// Create a render target view
			ID3D11Texture2D* pBackBuffer = nullptr;
			hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			if (FAILED(hr))
			{
				return hr;
			}

			hr = pDefaultDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView);
			aSAFE_RELEASE(pBackBuffer);
			if (FAILED(hr))
			{
				return hr;
			}

			// Create depth stencil texture
			D3D11_TEXTURE2D_DESC descDepth;
			ZeroMemory(&descDepth, sizeof(descDepth));
			descDepth.Width = WINWIDTH;
			descDepth.Height = WINHEIGHT;
			descDepth.MipLevels = 0;
			descDepth.ArraySize = 1;
			descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDepth.SampleDesc.Count = 1;
			descDepth.SampleDesc.Quality = 0;
			descDepth.Usage = D3D11_USAGE_DEFAULT;
			descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			descDepth.CPUAccessFlags = 0;
			descDepth.MiscFlags = 0;
			hr = pDefaultDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);
			if (FAILED(hr))
				return hr;
			// Create the depth stencil view
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = descDepth.Format;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;
			hr = pDefaultDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthStencilView);
			if (FAILED(hr))
				return hr;

			pImmediateContext->OMSetRenderTargets(1,
				&pRenderTargetView, pDepthStencilView);

			// Setup the viewport
			D3D11_VIEWPORT vp;
			vp.Width = (FLOAT)WINWIDTH;
			vp.Height = (FLOAT)WINHEIGHT;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			pImmediateContext->RSSetViewports(1, &vp);

			if (FAILED(hr))
				return hr;

			// Create the sample state
			D3D11_SAMPLER_DESC sampDesc;
			ZeroMemory(&sampDesc, sizeof(sampDesc));
			sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

			sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sampDesc.MipLODBias = 0.0f;
			sampDesc.MaxAnisotropy = 1;
			sampDesc.MinLOD = 0;
			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
			hr = pDefaultDevice->CreateSamplerState(&sampDesc, &pSamplerLinear);
			if (FAILED(hr))
			{
				return hr;
			}


			hr = pDefaultDevice->CreateSamplerState(&sampDesc, &pSamplerPoint);
			if (FAILED(hr))
			{
				return hr;
			}
			pImmediateContext->PSSetSamplers(0, 1, &pSamplerLinear);

			D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
			ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

			// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
			// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
			depthDisabledStencilDesc.DepthEnable = false;
			depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			depthDisabledStencilDesc.StencilEnable = true;
			depthDisabledStencilDesc.StencilReadMask = 0xFF;
			depthDisabledStencilDesc.StencilWriteMask = 0xFF;
			depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			// Create the state using the device.
			hr = pDefaultDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &pDisableDepthState);
			if (FAILED(hr))
			{
				return hr;
			}

			depthDisabledStencilDesc.DepthEnable = true;
			hr = pDefaultDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &pEnableDepthState);
			if (FAILED(hr))
			{
				return hr;
			}
			//Create blend states.
			D3D11_BLEND_DESC blendDesc;
			ZeroMemory(&blendDesc, sizeof(blendDesc));
			blendDesc.RenderTarget[0].BlendEnable = true;
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

			hr = pDefaultDevice->CreateBlendState(&blendDesc, &pEnableBlending);
			if (FAILED(hr))
			{
				return hr;
			}

			blendDesc.RenderTarget[0].BlendEnable = false;
			hr = pDefaultDevice->CreateBlendState(&blendDesc, &pDisableBlending);
			if (FAILED(hr))
			{
				return hr;
			}
			//set blend state. Enabled as default.
			float rgba[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			pImmediateContext->OMSetBlendState(pEnableBlending, rgba, 0xffffffff);

			//Create deffered context
			hr = pDefaultDevice->CreateDeferredContext(0, &pDeferredContext);
			if (FAILED(hr))
			{
				return hr;
			}
			return S_OK;
		}

		static void shutdown()
		{
			aSAFE_RELEASE(pDefaultDevice);
			aSAFE_RELEASE(pImmediateContext);
			aSAFE_RELEASE(pDeferredContext);
			aSAFE_RELEASE(pSwapChain);
			aSAFE_RELEASE(pRenderTargetView);
			aSAFE_RELEASE(pDepthStencil);
			aSAFE_RELEASE(pDepthStencilView);
			aSAFE_RELEASE(pTextureRV);
			aSAFE_RELEASE(pSamplerLinear);
			aSAFE_RELEASE(pSamplerPoint);
			aSAFE_RELEASE(pEnableDepthState);
			aSAFE_RELEASE(pDisableDepthState);
			aSAFE_RELEASE(pEnableBlending);
			aSAFE_RELEASE(pDisableBlending);
		}

		static ID3D11Device* getDevice(enum MANAGED_DEVICE_TYPE deviceEnum)
		{
			switch (deviceEnum)
			{
			case MANAGED_DEVICE_TYPE::DEFAULT_DEVICE:
				return pDefaultDevice;
			}
			return nullptr;
		}
		static ID3D11DeviceContext* getContext(enum MANAGED_CONTEXT_TYPE contextEnum)
		{
			switch (contextEnum)
			{
			case MANAGED_CONTEXT_TYPE::DEFAULT_CONTEXT:
			case MANAGED_CONTEXT_TYPE::IMMEDIATE_CONTEXT:
				return pImmediateContext;
			case MANAGED_CONTEXT_TYPE::DEFFERED_CONTEXT:
				return pDeferredContext;
			}
			return nullptr;
		}
		static ID3D11DepthStencilView* getDepthStencilView(enum MANAGED_DEPTH_STENCIL_VIEW_TYPE depthStencilViewEnum)
		{
			switch (depthStencilViewEnum)
			{
			case DEFAULT_DEPTH_STENCIL_VIEW:
				return pDepthStencilView;
			}
			return nullptr;
		}

		static void disableDepth(ID3D11DeviceContext* context)
		{
			context->OMSetDepthStencilState(pDisableDepthState, 1);
		}

		static void enableDepth(ID3D11DeviceContext* context)
		{
			context->OMSetDepthStencilState(pEnableDepthState, 1);
		}

		static void setMainRenderTarget()
		{
			pImmediateContext->OMSetRenderTargets(1, 
				&pRenderTargetView, pDepthStencilView);
		}

		static void clearRenderTarget()
		{
			float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
			pImmediateContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
			pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		}
		static void present(bool vSync)
		{
			auto nowTime = clock.now();
			static const __int64 singleCount = std::chrono::seconds(1).count();
			if (std::chrono::duration_cast<std::chrono::seconds>(nowTime - lastTime).count() >= singleCount)
			{
				framePerSecond = frameCounter;
				frameCounter = 0;
				lastTime = nowTime;
			}
			else
			{
				++frameCounter;
			}
			if (vSync == true)
				pSwapChain->Present(1, 0);
			else
				pSwapChain->Present(0, 0);
		}

		static size_t getFps()
		{
			return framePerSecond;
		}

		void setSampler(ID3D11DeviceContext* context,unsigned slot = 0,enum MANAGED_SAMPLER_TYPE samplerEnum = MANAGED_SAMPLER_TYPE::DEFAULT_SAMPLER)
		{
			switch (samplerEnum)
			{
			case MANAGED_SAMPLER_TYPE::DEFAULT_SAMPLER:
			case MANAGED_SAMPLER_TYPE::LINEAR_SAMPLER:
				context->PSSetSamplers(slot, 1, &pSamplerLinear);
				break;
			case MANAGED_SAMPLER_TYPE::POINT_SAMPLER:
				context->PSSetSamplers(slot, 1, &pSamplerPoint);
				break;
			}
		}
	private:
		static D3D_DRIVER_TYPE driverType;
		static D3D_FEATURE_LEVEL featureLevel;
		static ID3D11Device* pDefaultDevice;
		static ID3D11DeviceContext* pImmediateContext;
		static ID3D11DeviceContext* pDeferredContext;
		static IDXGISwapChain* pSwapChain;
		static ID3D11RenderTargetView* pRenderTargetView;
		static ID3D11Texture2D* pDepthStencil;
		static ID3D11DepthStencilView* pDepthStencilView;
		static ID3D11ShaderResourceView* pTextureRV;
		static ID3D11SamplerState* pSamplerLinear;
		static ID3D11SamplerState* pSamplerPoint;
		static ID3D11DepthStencilState* pEnableDepthState;
		static ID3D11DepthStencilState* pDisableDepthState;
		static ID3D11BlendState* pEnableBlending;
		static ID3D11BlendState* pDisableBlending;
		static std::chrono::high_resolution_clock  clock;
		static std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
		static size_t framePerSecond;
		static size_t frameCounter;
	};
}