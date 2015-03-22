#pragma once
#include"../../common/CommonDxSupport.hpp"
#include"WindowClass.hpp"
#include"D3DClass.hpp"
#include"ManagedTypes.hpp"
#include<unordered_map>

namespace aMazing
{
	class D3DManager
	{
	public:
		D3DManager() = delete;

		static HRESULT Initialize()
		{
			HRESULT hr = E_FAIL;
			hr = defaultDeviceAndContext.Initialize(WindowClass::getWindowHandler());
			if (FAILED(hr))
			{
				return hr;
			}
			return S_OK;
		}

		static ID3D11Device* getDevice(enum MANAGED_DEVICE_TYPE deviceEnum)
		{
			switch (deviceEnum)
			{
			case MANAGED_DEVICE_TYPE::DEFAULT_DEVICE:
				return defaultDeviceAndContext.getDevice();
			}
			return nullptr;
		}
		static ID3D11DeviceContext* getContext(enum MANAGED_CONTEXT_TYPE contextEnum)
		{
			switch (contextEnum){
			case MANAGED_CONTEXT_TYPE::DEFAULT_CONTEXT:
			case MANAGED_CONTEXT_TYPE::IMMEDIATE_CONTEXT:
				return defaultDeviceAndContext.getContext();
			case MANAGED_CONTEXT_TYPE::DEFFERED_CONTEXT:
					return nullptr;
			}
			return nullptr;
		}
		static ID3D11DepthStencilView* getDepthStencilView(enum MANAGED_DEPTH_STENCIL_VIEW_TYPE depthStencilViewEnum)
		{
			switch (depthStencilViewEnum)
			{
			case DEFAULT_DEPTH_STENCIL_VIEW:
				return defaultDeviceAndContext.getDepthStencilView();
			}
			return defaultDeviceAndContext.getDepthStencilView();
		}

		static void disableDepth()
		{
			defaultDeviceAndContext.DisableDepth();
		}

		static void enableDepth()
		{
			defaultDeviceAndContext.EnableDepth();
		}

		static void clearRenderTarget()
		{
			defaultDeviceAndContext.clearRenderTarget();
		}
		static void present(bool vsync)
		{
			defaultDeviceAndContext.Present(vsync);
		}
	private:
		static D3DClass defaultDeviceAndContext;
	};
}