#pragma once

#include<D3D11.h>
#include<D3DX11.h>
#include<D3DX11tex.h>
#include"GPUConstantBuffer.hpp"
#include"GPUVerticesBuffer.hpp"
#include"MutableString.h"
#include"TgaLoader.hpp"
#include<xnamath.h>

class TextureClass
{
public:
	TextureClass();
	~TextureClass();

	void Shutdown();

	HRESULT LoadFile(ID3D11Device* device,
		ID3D11DeviceContext* context,
		MutableString&& filename);
	HRESULT LoadMemory(ID3D11Device* device,
		ID3D11DeviceContext* context, 
		std::size_t width,
		std::size_t height,
		void* ptr, 
		std::size_t size);
	//make current texture chess board.
	HRESULT beChessBoard(ID3D11Device* device,
		ID3D11DeviceContext* context);

	bool bindVS(ID3D11Device* device,
		ID3D11DeviceContext* context, 
		unsigned int textureSlot);
	bool bindPS(ID3D11Device* device,
		ID3D11DeviceContext* context, 
		unsigned int textureSlot);
	bool isInited() const;
private:
	bool is_init;
	ID3D11ShaderResourceView* SRV;
	const static unsigned short chessBoardWidth = 512;
	const static unsigned short chessBoardHeight = 512;
	struct chessBoardDatz{
		explicit chessBoardDatz()
		{
			bool flag = false;
			for (int i = 0; i < chessBoardWidth; i++)
			{
				for (int j = 0; j < chessBoardHeight; j++)
				{
					if (((i & 0x20) ^ (j & 0x20)) == 0)
					{
						chessBoardData[(chessBoardWidth * j + i) * 4] = 0;
						chessBoardData[(chessBoardWidth * j + i) * 4 + 1] = 0;
						chessBoardData[(chessBoardWidth * j + i) * 4 + 2] = 0;
						chessBoardData[(chessBoardWidth * j + i) * 4 + 3] = 255;
					}
					else
					{
						chessBoardData[(chessBoardWidth * j + i) * 4] = 255;
						chessBoardData[(chessBoardWidth * j + i) * 4 + 1] = 0;
						chessBoardData[(chessBoardWidth * j + i) * 4 + 2] = 255;
						chessBoardData[(chessBoardWidth * j + i) * 4 + 3] = 255;
					}
				}
			}
		}
		unsigned char chessBoardData[chessBoardWidth * chessBoardHeight * 4];
	};
	const static chessBoardDatz chessBoardData;
};
