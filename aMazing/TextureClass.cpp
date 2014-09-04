#include "TextureClass.h"


TextureClass::TextureClass()
{
	SRV = nullptr;
}


TextureClass::~TextureClass()
{
	if (SRV != nullptr)
	{
		delete SRV;
		SRV = nullptr;
	}
}

void TextureClass::Shutdown()
{
	if (SRV != nullptr)
	{
		SRV->Release();
		SRV = nullptr;
	}
}

bool TextureClass::LoadFile(ID3D11Device* device,
	ID3D11DeviceContext* context,
	MutableString&& filename)
{
	HRESULT hr;
	if (SRV != nullptr)
	{
		return false;
	}
	hr = D3DX11CreateShaderResourceViewFromFile(
		device, 
		filename.getWideString().c_str(),
		nullptr,
		nullptr,
		&SRV,
		nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool TextureClass::LoadMemory(ID3D11Device* device,
	ID3D11DeviceContext* context,
	void* ptr,
	std::size_t size)
{
	HRESULT hr;
	if (SRV != nullptr)
	{
		return false;
	}
	hr = D3DX11CreateShaderResourceViewFromMemory(
		device,
		ptr,
		size,
		nullptr,
		nullptr,
		&SRV,
		nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}


bool TextureClass::bindVS(ID3D11Device* device,
	ID3D11DeviceContext* context, 
	unsigned int textureSlot)
{
	context->VSSetShaderResources(textureSlot, 1, &SRV);
	return true;
}

bool TextureClass::bindPS(ID3D11Device* device,
	ID3D11DeviceContext* context,
	unsigned int textureSlot)
{
	context->PSSetShaderResources(textureSlot, 1, &SRV);
	return true;
}