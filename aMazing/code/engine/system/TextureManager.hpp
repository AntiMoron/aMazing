#pragma once

#include<vector>
#include"TextureObject.hpp"
#include"thread/aThreadSafeSingleton.hpp"
namespace aMazing
{
	class TextureManager : public aThreadSafeSingleton<TextureManager>
	{
	private:
		TextureManager(){}
		friend class aThreadSafeSingleton<TextureManager>;
	public:
		HRESULT addTexture(ID3D11Device* device,
			MutableString&& filename);
		HRESULT addChessBoardTexture(ID3D11Device* device);
		//There won't need a delete function here.
		//For textures are set manually before compiling.

		TextureObject* getTexture(std::size_t index);
	private:
		std::vector<std::unique_ptr<TextureObject> > m_vec;
	};
}

#define TEXTURE (aMazing::TextureManager::getInstance())
