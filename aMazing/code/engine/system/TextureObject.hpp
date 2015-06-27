#pragma once
#include"../../common/CommonDxSupport.hpp"
#include<D3DX11tex.h>
#include"GPUConstantBuffer.hpp"
#include"../data/TgaLoader.hpp"
#include"../containers/MutableString.hpp"

namespace aMazing
{
	class TextureObject
	{
	public:
		TextureObject()
		{
			shaderResourceView = nullptr;
			bIsInit = false;
		}

		virtual ~TextureObject()
		{
			aSAFE_RELEASE(shaderResourceView);
		}

		/*
		 * Load Texture from a file
		 * 
		*/
		HRESULT LoadFile(ID3D11Device* device,
			MutableString&& filename)
		{
			HRESULT hr;
			if (shaderResourceView != nullptr)
			{
				return E_FAIL;
			}
			std::string filePath = filename.getMultiByteString();
			if (TGA::TgaLoader::getLoader().judgeTga(filePath.c_str()))
			{
				TGA::TgaData data;
				TGALOAD(filePath.c_str(), &data);
				long pixelCount = long(data.getHeight()) * long(data.getWidth());

				hr = LoadMemory(device, data.getWidth(), data.getHeight(),
					data.getColDataPtr(),
					sizeof(XMFLOAT4)* pixelCount);
				if (FAILED(hr))
				{
					return E_FAIL;
				}
			}
			else
			{
				hr = D3DX11CreateShaderResourceViewFromFile(
					device,
					filename.getWideString().c_str(),
					nullptr,
					nullptr,
					&shaderResourceView,
					nullptr);
				if (FAILED(hr))
				{
					return E_FAIL;
				}
			}
			bIsInit = true;
			return S_OK;
		}
		/*
		 * Load Texture from a period of memory.
		 * @param device Dx11 Device.
		 * @param context Dx11 Context.
		 * @param width width of the texture.
		 * @param height height of the texture.
		 * @param ptr  the pointer that points the texture data.
		 * @param size the size of the texture data.
		*/
		HRESULT LoadMemory(ID3D11Device* device,
			std::size_t width,
			std::size_t height,
			void* ptr, 
			std::size_t size)
		{
			HRESULT hr;
			if (shaderResourceView != nullptr)
			{
				return E_FAIL;
			}

			D3D11_TEXTURE2D_DESC textureDesc;
			ZeroMemory(&textureDesc, sizeof(textureDesc));
			textureDesc.Width = width;
			textureDesc.Height = height;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = 0;

			ID3D11Texture2D* tex2d = nullptr;
			D3D11_SUBRESOURCE_DATA resourceData;
			ZeroMemory(&resourceData, sizeof(resourceData));
			resourceData.pSysMem = ptr;
			resourceData.SysMemPitch = width * 4;
			resourceData.SysMemSlicePitch = width * height * 4;
			hr = device->CreateTexture2D(&textureDesc, &resourceData, &tex2d);
			if (FAILED(hr))
			{
				return E_FAIL;
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.Format = textureDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = 1;
			hr = device->CreateShaderResourceView(tex2d, &srvDesc, &shaderResourceView);
			if (FAILED(hr))
			{
				return E_FAIL;
			}
			if (shaderResourceView == nullptr)
			{
				return E_FAIL;
			}
			//release the texture2d object
			aSAFE_RELEASE(tex2d);
			bIsInit = true;
			return S_OK;
		}

		//make current texture chess board.
		HRESULT beChessBoard(ID3D11Device* device)
		{
			HRESULT hr;
			hr = LoadMemory(device,
				defaultWidth,
				defaultHeight,
				reinterpret_cast<void*>(const_cast<unsigned char*>(defaultData.chessBoardData)),
				sizeof(unsigned char)* defaultWidth * defaultHeight * 4);
			if (FAILED(hr))
			{
				return hr;
			}
			bIsInit = true;
			return hr;
		}

		virtual void bindVS(ID3D11DeviceContext* context, 
			unsigned int textureSlot) const
		{
			context->VSSetShaderResources(textureSlot, 1, &shaderResourceView);
		}

		virtual void bindPS(ID3D11DeviceContext* context,
			unsigned int textureSlot) const
		{
			context->PSSetShaderResources(textureSlot, 1, &shaderResourceView);
		}

		bool isInited() const
		{
			return bIsInit;
		}
	protected:
		bool bIsInit;
		ID3D11ShaderResourceView* shaderResourceView;
	private:
		const static unsigned short defaultWidth = 512;
		const static unsigned short defaultHeight = 512;
		struct wrappedColorDatz{
			explicit wrappedColorDatz()
			{
				for (int i = 0; i < defaultWidth; i++)
				{
					for (int j = 0; j < defaultHeight; j++)
					{
						//the gray color
						gray[(defaultWidth * j + i) * 4] = 160;
						gray[(defaultWidth * j + i) * 4 + 1] = 160;
						gray[(defaultWidth * j + i) * 4 + 2] = 160;
						gray[(defaultWidth * j + i) * 4 + 3] = 255;
						//the white color
						white[(defaultWidth * j + i) * 4 ] = 255;
						white[(defaultWidth * j + i) * 4 + 1] = 255;
						white[(defaultWidth * j + i) * 4 + 2] = 255;
						white[(defaultWidth * j + i) * 4 + 3] = 255;
						//the black color
						black[(defaultWidth * j + i) * 4] = 0;
						black[(defaultWidth * j + i) * 4 + 1] = 0;
						black[(defaultWidth * j + i) * 4 + 2] = 0;
						black[(defaultWidth * j + i) * 4 + 3] = 255;
						//red color
						red[(defaultWidth * j + i) * 4] = 255;
						red[(defaultWidth * j + i) * 4 + 1] = 0;
						red[(defaultWidth * j + i) * 4 + 2] = 0;
						red[(defaultWidth * j + i) * 4 + 3] = 255;
						//blue color
						blue[(defaultWidth * j + i) * 4] = 0;
						blue[(defaultWidth * j + i) * 4 + 1] = 0;
						blue[(defaultWidth * j + i) * 4 + 2] = 255;
						blue[(defaultWidth * j + i) * 4 + 3] = 255;
						//green color
						green[(defaultWidth * j + i) * 4] = 0;
						green[(defaultWidth * j + i) * 4 + 1] = 255;
						green[(defaultWidth * j + i) * 4 + 2] = 0;
						green[(defaultWidth * j + i) * 4 + 3] = 255;
						if (((i & 0x40) ^ (j & 0x40)) == 0)
						{
							chessBoardData[(defaultWidth * j + i) * 4] = 255;
							chessBoardData[(defaultWidth * j + i) * 4 + 1] = 0;
							chessBoardData[(defaultWidth * j + i) * 4 + 2] = 0;
							chessBoardData[(defaultWidth * j + i) * 4 + 3] = 255;
						}
						else
						{
							chessBoardData[(defaultWidth * j + i) * 4] = 255;
							chessBoardData[(defaultWidth * j + i) * 4 + 1] = 255;
							chessBoardData[(defaultWidth * j + i) * 4 + 2] = 255;
							chessBoardData[(defaultWidth * j + i) * 4 + 3] = 255;
						}
					}
				}
			}
			unsigned char gray[defaultWidth * defaultHeight * 4];
			unsigned char white[defaultWidth * defaultHeight * 4];
			unsigned char black[defaultWidth * defaultHeight * 4];
			unsigned char red[defaultWidth * defaultHeight * 4];
			unsigned char blue[defaultWidth * defaultHeight * 4];
			unsigned char green[defaultWidth * defaultHeight * 4];
			unsigned char chessBoardData[defaultWidth * defaultHeight * 4];
		};
		const static wrappedColorDatz defaultData;
	};
}
