#pragma once
#include"../system/thread/aThreadSafeSingleton.hpp"
#include"../exception/FailureException.hpp"
#include"fontface.hpp"
#include"facecache.hpp"
#include"textcache.hpp"
#include<cstring>

namespace aMazing
{
	using namespace detail;
	class FontEngine : public aThreadSafeSingleton<FontEngine>
	{
	private:
		explicit FontEngine() throw(FailureException)
		{
			ftError = FT_Init_FreeType(&fontLibrary);
			if (!!ftError)
			{
				throw FailureException("Font initialization failed.");
			}
		}
		friend class aThreadSafeSingleton<FontEngine>;
		detail::FaceCache faceCache;
		detail::TextCache textCache;
	public:
		~FontEngine()
		{
			FT_Done_FreeType(fontLibrary);
		}
		const FontTexture* getFontBitmap(wchar_t ch,
			int size,
			const char* fontName,
			int resX, int resY) aNOEXCEPT
		{
			size_t cur = 0;
			if (textCache.findCharacter(ch, fontName) == -1)
			{
				FontBitmap* fontBitmap = getRawFontBitmap(ch,
					fontName, size, resX, resY);
				textCache.applyCharacter(ch, fontName, fontBitmap);
				delete fontBitmap;
			}
			return textCache.getFontResult(ch, fontName);
		}

		//fontName where the font exist.
		//text the text gonna be rendered
		//size font size
		//resX resY resolution of the font.
		FontBitmap* getRawFontBitmap(wchar_t ch, 
			const char* fontName,
			int size,
			int resX, int resY) aNOEXCEPT
		{
			FT_Face face = faceCache[fontName];
			FontBitmap* result = new FontBitmap;
			ftError = FT_Set_Char_Size(face, 0, size * 64, resX, resY);
			if (ftError)
				return nullptr;
			FT_UInt glyph_index;
			FT_GlyphSlot slot = face->glyph;

			glyph_index = FT_Get_Char_Index(face, ch);
			ftError = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
			if (ftError)
				return nullptr;
			ftError = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
			if (ftError)
				return nullptr;
			auto& ret = *result;
			ret.size = size;
			ret.width = slot->bitmap.width;
			ret.height = slot->bitmap.rows;
			ret.bitmapLeft = slot->metrics.horiBearingX / 64;
			ret.bitmapTop = slot->metrics.vertBearingY / 64;
			ret.buffer = new unsigned char[ret.width * ret.height * 4];
			size_t totalPixels = ret.width * ret.height;
			for (size_t cur = 0; cur < totalPixels; ++cur)
			{
				ret.buffer[cur * 4] = slot->bitmap.buffer[cur];
				ret.buffer[cur * 4 + 1] = slot->bitmap.buffer[cur];
				ret.buffer[cur * 4 + 2] = slot->bitmap.buffer[cur];
				ret.buffer[cur * 4 + 3] = slot->bitmap.buffer[cur];
			}
			ret.nextPenX = slot->advance.x >> 6;
			ret.nextPenY = slot->advance.y >> 6;
			return result;
		}
	};
}
#define FONT (aMazing::FontEngine::getInstance())
