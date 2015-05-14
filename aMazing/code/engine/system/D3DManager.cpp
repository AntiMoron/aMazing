#include"D3DManager.hpp"
using namespace aMazing;

//D3DClass D3DManager::defaultDeviceAndContext;
D3D_DRIVER_TYPE D3DManager::driverType = D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL D3DManager::featureLevel;
ID3D11Device* D3DManager::pDefaultDevice = nullptr;
ID3D11DeviceContext* D3DManager::pImmediateContext = nullptr;
ID3D11DeviceContext* D3DManager::pDeferredContext = nullptr;
IDXGISwapChain* D3DManager::pSwapChain = nullptr;
ID3D11RenderTargetView* D3DManager::pRenderTargetView = nullptr;
ID3D11Texture2D* D3DManager::pDepthStencil = nullptr;
ID3D11DepthStencilView* D3DManager::pDepthStencilView = nullptr;
ID3D11ShaderResourceView* D3DManager::pTextureRV = nullptr;
ID3D11SamplerState* D3DManager::pSamplerLinear = nullptr;
ID3D11SamplerState* D3DManager::pSamplerPoint = nullptr;
ID3D11DepthStencilState* D3DManager::pEnableDepthState = nullptr;
ID3D11DepthStencilState* D3DManager::pDisableDepthState = nullptr;
ID3D11BlendState* D3DManager::pEnableBlending = nullptr;
ID3D11BlendState* D3DManager::pDisableBlending = nullptr;
std::chrono::high_resolution_clock  D3DManager::clock;
std::chrono::time_point<std::chrono::high_resolution_clock> D3DManager::lastTime;
D3D11_VIEWPORT D3DManager::viewPort;
size_t D3DManager::framePerSecond = 0;
size_t D3DManager::frameCounter = 0;