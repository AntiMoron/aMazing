#include "D3DClass.h"


D3DClass::D3DClass()
{
	g_driverType = D3D_DRIVER_TYPE_NULL;
	g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	g_pd3dDevice = nullptr;
	g_pImmediateContext = nullptr;
	g_pSwapChain = nullptr;
	g_pRenderTargetView = nullptr;
	g_pDepthStencil = nullptr;
	g_pDepthStencilView = nullptr;
	g_pTextureRV = nullptr;
	g_pSamplerLinear = nullptr;
}


D3DClass::~D3DClass()
{
}

HRESULT D3DClass::Initialize(HWND hwnd)
{
	HRESULT hr;
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
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
	sd.SampleDesc.Count = 4;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = WINWIDTH;
	descDepth.Height = WINHEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 4;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)WINWIDTH;
	vp.Height = (FLOAT)WINHEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);

	if (FAILED(hr))
		return E_FAIL;

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
	if (FAILED(hr))
		return hr;

	g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
	return S_OK;
}
HRESULT D3DClass::Shutdown()
{
	if (g_pImmediateContext)
	{
		g_pImmediateContext->ClearState();
	}
	if (g_pSamplerLinear)
	{
		g_pSamplerLinear->Release();
		g_pSamplerLinear = nullptr;
	}
	if (g_pTextureRV)
	{
		g_pTextureRV->Release();
		g_pTextureRV = nullptr;
	}
	if (g_pDepthStencil)
	{
		g_pDepthStencil->Release();
		g_pDepthStencil = nullptr;
	}
	if (g_pDepthStencilView)
	{
		g_pDepthStencilView->Release();
		g_pDepthStencilView = nullptr;
	}
	if (g_pRenderTargetView)
	{
		g_pRenderTargetView->Release();
		g_pRenderTargetView = nullptr;
	}
	if (g_pSwapChain)
	{
		g_pSwapChain->Release();
		g_pSwapChain = nullptr;
	}
	if (g_pImmediateContext)
	{
		g_pImmediateContext->Release();
		g_pImmediateContext = nullptr;
	}
	if (g_pd3dDevice)
	{
		g_pd3dDevice->Release();
		g_pd3dDevice = nullptr;
	}
	return S_OK;
}

ID3D11Device* D3DClass::getDevice()
{
	return g_pd3dDevice;
}
ID3D11DeviceContext* D3DClass::getContext()
{
	return g_pImmediateContext;
}
ID3D11RenderTargetView* D3DClass::getRenderTargetView()
{
	return g_pRenderTargetView;
}
ID3D11DepthStencilView* D3DClass::getDepthStencilView()
{
	return g_pDepthStencilView;
}

void D3DClass::setRenderTarget()
{
	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
}

void D3DClass::Present(bool VSync)
//Vsync mean whether enable Vertical-Sync
{
	if (VSync == true)
		g_pSwapChain->Present(1, 0);
	else
		g_pSwapChain->Present(0, 0);
}