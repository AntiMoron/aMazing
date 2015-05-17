#pragma once

#include"../../common/CommonDxSupport.hpp"
#include<D3DX11tex.h>
#include"GPUConstantBuffer.hpp"
#include"GPUVerticesBuffer.hpp"
#include"../data/TgaLoader.hpp"
#include"../containers/MutableString.hpp"
namespace aMazing
{
	class TextureObject
	{
	public:
		TextureObject();
		~TextureObject();

		/*
		 * Load Texture from a file
		 * 
		*/
		HRESULT LoadFile(ID3D11Device* device,
			MutableString&& filename);
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
			std::size_t size);
		//make current texture chess board.
		HRESULT beChessBoard(ID3D11Device* device);

		bool bindVS(ID3D11DeviceContext* context, 
			unsigned int textureSlot);
		bool bindPS(ID3D11DeviceContext* context, 
			unsigned int textureSlot);
		bool isInited() const;
	private:
		bool is_init;
		ID3D11ShaderResourceView* SRV;
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
