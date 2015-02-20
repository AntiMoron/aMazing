#pragma once

#include<vector>
#include"TextureClass.hpp"
namespace aMazing
{
	class TextureManager
	{
	public:
		static TextureManager& getInstance();

		HRESULT addTexture(ID3D11Device* device,
			ID3D11DeviceContext* context,
			MutableString&& filename);
		HRESULT addChessBoardTexture(ID3D11Device* device,
			ID3D11DeviceContext* context);
		//There won't need a delete function here.
		//For textures are set manually before compiling.

		TextureClass* getTexture(std::size_t index);
	private:
		TextureManager();
		~TextureManager();
	
		std::vector<std::unique_ptr<TextureClass> > m_vec;
	};
}

#define TEXTURE (aMazing::TextureManager::getInstance())