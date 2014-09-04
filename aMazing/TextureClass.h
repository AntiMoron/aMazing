#pragma once

#include<D3D11.h>
#include<D3DX11.h>
#include"GPUConstantBuffer.hpp"
#include"GPUVerticesBuffer.hpp"
#include"MutableString.h"

class TextureClass
{
public:
	TextureClass();
	~TextureClass();

	void Shutdown();

	bool LoadFile(ID3D11Device* device,
		ID3D11DeviceContext* context,
		MutableString&& filename);
	bool LoadMemory(ID3D11Device* device,
		ID3D11DeviceContext* context, 
		void* ptr, 
		std::size_t size);

	bool bindVS(ID3D11Device* device,
		ID3D11DeviceContext* context, 
		unsigned int textureSlot);
	bool bindPS(ID3D11Device* device,
		ID3D11DeviceContext* context, 
		unsigned int textureSlot);
private:
	ID3D11ShaderResourceView* SRV;
};

