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
};

