#pragma once

#include"GlobalWindow.hpp"
#include"../system/TextureObject.hpp"
#include"../../common/CommonDxSupport.hpp"
#include"../../common/CommonDef.hpp"

namespace aMazing
{
	class FrameBuffer : public TextureObject
	{
	public:
		FrameBuffer()
		{
			renderTargetTexture = nullptr;
			renderTargetView = nullptr;
			pDepthStencil = nullptr;
			depthStencilView = nullptr;
		}
		~FrameBuffer()
		{
			aSAFE_RELEASE(renderTargetTexture);
			aSAFE_RELEASE(renderTargetView);
			aSAFE_RELEASE(pDepthStencil);
			aSAFE_RELEASE(depthStencilView);
		}

		HRESULT initialize(ID3D11Device* device,
			unsigned short imageWidth = -1, 
			unsigned short imageHeight = -1)
		{
			HRESULT hr;
			if (imageWidth == unsigned short(-1))
				imageWidth = RESWIDTH;
			if (imageHeight == unsigned short(-1))
				imageHeight = RESHEIGHT;
			viewPort.MaxDepth = 1.0f;
			viewPort.MinDepth = 0.0f;
			viewPort.TopLeftX = 0.0f;
			viewPort.TopLeftY = 0.0f;
			viewPort.Width = imageWidth;
			viewPort.Height = imageHeight;
			D3D11_TEXTURE2D_DESC textureDesc;
			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			ZeroMemory(&textureDesc, sizeof(textureDesc));
			ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
			ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
			textureDesc.Width = imageWidth;
			textureDesc.Height = imageHeight;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = 0;
			hr = device->CreateTexture2D(&textureDesc, NULL, &renderTargetTexture);
			if (FAILED(hr))
			{
				return hr;
			}
			renderTargetViewDesc.Format = textureDesc.Format;
			renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			renderTargetViewDesc.Texture2D.MipSlice = 0;
			hr = device->CreateRenderTargetView(renderTargetTexture, 
				&renderTargetViewDesc, &renderTargetView);
			if (FAILED(hr))
			{
				return hr;
			}
			shaderResourceViewDesc.Format = textureDesc.Format;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;
			hr = device->CreateShaderResourceView(renderTargetTexture, &shaderResourceViewDesc, &shaderResourceView);
			if (FAILED(hr))
			{
				return hr;
			}

			D3D11_TEXTURE2D_DESC descDepth;
			ZeroMemory(&descDepth, sizeof(descDepth));
			descDepth.Width = imageWidth;
			descDepth.Height = imageHeight;
			descDepth.MipLevels = 1;
			descDepth.ArraySize = 1;
			descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDepth.SampleDesc.Count = 1;
			descDepth.SampleDesc.Quality = 0;
			descDepth.Usage = D3D11_USAGE_DEFAULT;
			descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			descDepth.CPUAccessFlags = 0;
			descDepth.MiscFlags = 0;
			hr = device->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
			if (FAILED(hr))
				return hr;

			// Create the depth stencil view
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = descDepth.Format;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;
			hr = device->CreateDepthStencilView(pDepthStencil, 
				&descDSV, &depthStencilView);
			if (FAILED(hr))
				return hr;

			bIsInit = true;
			return S_OK;
		}

		void setRenderTarget(ID3D11DeviceContext* context)
		{
			static ID3D11ShaderResourceView* null[] = {nullptr};
			context->PSSetShaderResources(0, 1, null);
			context->OMSetRenderTargets(1,
				&renderTargetView, depthStencilView);
			context->RSSetViewports(1, &viewPort);
		}

		void clearRenderTarget(ID3D11DeviceContext* context)
		{
			static float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			context->ClearRenderTargetView(renderTargetView, clearColor);
			context->ClearDepthStencilView(depthStencilView,
				D3D11_CLEAR_DEPTH, 1.0f, NULL);
		}

		void bindVS(ID3D11DeviceContext* context,
			unsigned int textureSlot) const
		{
			static ID3D11RenderTargetView* null[] = { nullptr };
			context->OMSetRenderTargets(1, null, nullptr);
			context->VSSetShaderResources(textureSlot, 1, &shaderResourceView);
		}

		void bindPS(ID3D11DeviceContext* context,
			unsigned int textureSlot) const
		{
			static ID3D11RenderTargetView* null[] = {nullptr};
			context->OMSetRenderTargets(1, null, nullptr);
			context->PSSetShaderResources(textureSlot, 1, &shaderResourceView);
		}

		void clearDepthBuffer(ID3D11Device* device, ID3D11DeviceContext* context)
		{
			context->ClearDepthStencilView(depthStencilView,
				D3D11_CLEAR_DEPTH, 1.0f, NULL);
		}
	private:
		ID3D11Texture2D* renderTargetTexture;
		ID3D11RenderTargetView* renderTargetView;
		ID3D11Texture2D* pDepthStencil;
		ID3D11DepthStencilView* depthStencilView;
		D3D11_VIEWPORT viewPort;
	};
}
