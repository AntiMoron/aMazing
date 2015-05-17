#include "TextureObject.hpp"
using namespace aMazing;
const TextureObject::wrappedColorDatz TextureObject::defaultData;

TextureObject::TextureObject()
{
	SRV = nullptr;
	is_init = false;
}


TextureObject::~TextureObject()
{
		aSAFE_RELEASE(SRV);
}

HRESULT TextureObject::LoadFile(ID3D11Device* device,
	MutableString&& filename)
{
	HRESULT hr;
	if (SRV != nullptr)
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
			&SRV,
			nullptr);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
	}
	is_init = true;
	return S_OK;
}

HRESULT TextureObject::LoadMemory(ID3D11Device* device,
	std::size_t width,
	std::size_t height,
	void* ptr,
	std::size_t size)
{
	HRESULT hr;
	if (SRV != nullptr)
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
	hr = device->CreateTexture2D(&textureDesc,&resourceData,&tex2d);
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
	hr = device->CreateShaderResourceView(tex2d, &srvDesc, &SRV);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	if (SRV == nullptr)
	{
		return E_FAIL;
	}
	//release the texture2d object
	aSAFE_RELEASE(tex2d);
	is_init = true;
	return S_OK;
}

HRESULT TextureObject::beChessBoard(ID3D11Device* device)
{
	HRESULT hr;
	hr = LoadMemory(device,
		defaultWidth,
		defaultHeight,
		reinterpret_cast<void*>(const_cast<unsigned char*>(defaultData.chessBoardData)),
		sizeof(unsigned char) * defaultWidth * defaultHeight * 4);
	if (FAILED(hr))
	{
		return hr;
	}
	is_init = true;
	return hr;
}


bool TextureObject::bindVS(ID3D11DeviceContext* context, 
	unsigned int textureSlot)
{
	context->VSSetShaderResources(textureSlot, 1, &SRV);
	return true;
}

bool TextureObject::bindPS(ID3D11DeviceContext* context,
	unsigned int textureSlot)
{
	context->PSSetShaderResources(textureSlot, 1, &SRV);
	return true;
}

bool TextureObject::isInited()const
{
	return is_init;
}
