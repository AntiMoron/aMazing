#include "TextureManager.h"
TextureManager* TextureManager::instance = nullptr;

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}


TextureManager& TextureManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new TextureManager;
	}
	return *instance;
}

HRESULT TextureManager::addTexture(ID3D11Device* device,
	ID3D11DeviceContext* context,
	MutableString&& filename)
	//add a brand new texture into texture manager class.
{
	TextureClass* texture = new TextureClass;
	std::string path = filename.getMultiByteString();
	if (FAILED(texture->LoadFile(device, context, path)))
	{
		return E_FAIL;
	}
	m_vec.push_back(std::unique_ptr<TextureClass>(texture));
	return S_OK;
}

HRESULT TextureManager::addChessBoardTexture(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	TextureClass* texture = new TextureClass;
	if (FAILED(texture->beChessBoard(device,context)))
	{
		return E_FAIL;
	}
	m_vec.push_back(std::unique_ptr<TextureClass>(texture));
	return S_OK;
}

TextureClass* TextureManager::getTexture(std::size_t index)
//Get the pointer of a texture saved in texture manager class.
{
	if (index >= m_vec.size())
	{
		return nullptr;
	}
	return m_vec[index].get();
}