#pragma once
#include"fontglobal.hpp"
#include"../system/D3DManager.hpp"
#include"../system/TextureObject.hpp"
#include<unordered_map>
#include<functional>

namespace aMazing
{
	class FontTexture : public TextureObject
	{
	public:
		int getBitmapLeft() const aNOEXCEPT
		{
			  return bitmapLeft;
		}
		void setBitmapLeft(int l) aNOEXCEPT
		{
			bitmapLeft = l;
		}
		int getBitmapTop() const aNOEXCEPT
		{
			return bitmapTop;
		}
		void setBitmapTop(int t) aNOEXCEPT
		{
			bitmapTop = t;
		}
		int getNextPenX() const aNOEXCEPT
		{
			return nextPenX;
		}
		void setNextPenX(int x) aNOEXCEPT
		{
			nextPenX = x;
		}
		int getNextPenY() const aNOEXCEPT
		{
			return nextPenY;
		}
		void setNextPenY(int y) aNOEXCEPT
		{
			nextPenY = y;
		}
	private:
		int bitmapLeft;
		int bitmapTop;
		int nextPenX;
		int nextPenY;
	};

	namespace detail
	{
		class TextCache
		{
		private:
			std::unordered_map<size_t, FontTexture> textCache;
		public:
			TextCache(size_t lim = 5000)
			{
				limit = lim;
			}

			int findCharacter(wchar_t ch, const char* fontName) const aNOEXCEPT
			{
				  auto it = textCache.find(getFontHash(ch, fontName));
				  if (it == textCache.end())
				  {
					  return -1;
				  }
				  return 0;
			}

			void applyCharacter(wchar_t ch, 
				const char* fontName,
				FontBitmap* bitmap)
			{
				size_t hashCode = getFontHash(ch, fontName);
				auto it = textCache.find(hashCode);
				if (it == textCache.end())
				{
					if (textCache.size() + 1 >= limit)
					{
						textCache.erase(textCache.begin());
					}
					auto it = textCache.insert(
						std::pair<size_t, FontTexture>(hashCode,
						FontTexture()));
					FontTexture& tex = it.first->second;
					tex.LoadMemory(D3DManager::getDevice(DEFAULT_DEVICE),
						bitmap->width, bitmap->height,
						bitmap->buffer);
					tex.setBitmapLeft(bitmap->bitmapLeft);
					tex.setBitmapTop(bitmap->bitmapTop);
					tex.setNextPenX(bitmap->nextPenX);
					tex.setNextPenY(bitmap->nextPenY);
				}
			}
			const FontTexture* getFontResult(wchar_t ch,
				const char* fontName) const aNOEXCEPT
			{
				auto it = textCache.find(getFontHash(ch,fontName));
				return aAddressof(it->second);
			}
		private:
			size_t limit;
			size_t getFontHash(wchar_t ch, const char* fontName)
				const aNOEXCEPT
			{
				static std::hash<std::wstring> whasher;
				return (size_t(ch) ^ hashCStringLiteral(fontName));
			}
		};
	}
}
