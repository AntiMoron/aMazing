#pragma once
#include"fontglobal.hpp"
#include"../system/D3DManager.hpp"
#include"../system/TextureObject.hpp"
#include<unordered_map>
#include<functional>

namespace aMazing
{
	namespace detail
	{
		class TextCache
		{
		private:
			std::unordered_map<size_t, TextureObject> textCache;
		public:
			int findCharacter(wchar_t ch, const char* fontName) const aNOEXCEPT
			{
				  auto it = textCache.find(getFontHash(ch, fontName));
				  if (it == textCache.end())
				  {
					  return -1;
				  }
				  return 0;
			}

			bool applyCharacter(wchar_t ch, 
				const char* fontName,
				FontBitmap* bitmap)
			{
				size_t hashCode = getFontHash(ch, fontName);
				auto it = textCache.find(hashCode);
				if (it == textCache.end())
				{
					auto it = textCache.insert(
						std::pair<size_t, TextureObject>(hashCode,
						TextureObject()));
					TextureObject& tex = it.first->second;
					tex.LoadMemory(D3DManager::getDevice(DEFAULT_DEVICE),
						bitmap->width, bitmap->height,
						bitmap->buffer);
					return false;
				}
				return true;
			}
			const TextureObject* getFontResult(wchar_t ch,
				const char* fontName) const aNOEXCEPT
			{
				auto it = textCache.find(getFontHash(ch,fontName));
				return aAddressof(it->second);
			}
		private:
			size_t getFontHash(wchar_t ch, const char* fontName)
				const aNOEXCEPT
			{
				static std::hash<std::wstring> whasher;
				return (size_t(ch) ^ hashCStringLiteral(fontName));
			}
		};
	}
}
