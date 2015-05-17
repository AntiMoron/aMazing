#include "TextureManager.hpp"
using namespace aMazing;

HRESULT TextureManager::addTexture(ID3D11Device* device,
	MutableString&& filename)
	//add a brand new texture into texture manager class.
{
	TextureObject* texture = new TextureObject;
	std::string path = filename.getMultiByteString();
	if (FAILED(texture->LoadFile(device, path)))
	{
		return E_FAIL;
	}
	m_vec.push_back(std::unique_ptr<TextureObject>(texture));
	return S_OK;
}

HRESULT TextureManager::addChessBoardTexture(ID3D11Device* device)
{
	TextureObject* texture = new TextureObject;
	if (FAILED(texture->beChessBoard(device)))
	{
		return E_FAIL;
	}
	m_vec.push_back(std::unique_ptr<TextureObject>(texture));
	return S_OK;
}

TextureObject* TextureManager::getTexture(std::size_t index)
//Get the pointer of a texture saved in texture manager class.
{
	if (index >= m_vec.size())
	{
		return nullptr;
	}
	return m_vec[index].get();
}
