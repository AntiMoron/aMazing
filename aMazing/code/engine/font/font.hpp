#pragma once
#include"../../common/CommonDef.hpp"
#include"../../common/CommonFunction.hpp"
#include"../system/thread/aThreadSafeSingleton.hpp"
#include"../exception/FailureException.hpp"
#include"../containers/Directory.hpp"
#include"../../util/randomgen.hpp"
#include"fontface.hpp"
#include<memory>
#include<unordered_map>
#include<cstring>

namespace aMazing
{
	namespace
	{
		FT_Error ftError;
		FT_Library fontLibrary = nullptr;
	}

	namespace detail
	{
		class FaceCache
		{
		public:
			FaceCache(size_t lim = 1000)
			{
				limit = lim;
			}
			void addFace(const char* fileName)
			{
				aString _fileName = transformSplashes(fileName);
				_fileName = minimizeDirectory(_fileName);
				size_t hashCode = hashCStringLiteral(_fileName.c_str());
				size_t rawHashCode = hashCStringLiteral(fileName);
				auto it = faceCache.find(hashCode);
				if (it != faceCache.end())
				{
					if (hashCode != rawHashCode)
						faceCache[rawHashCode] = it->second;
				}
				else
				{
					if (faceCache.size() + 1 >= limit)
					{
						int count = 0;
						int id = Random.getInteger32(0, faceCache.size());
						for (auto it = faceCache.begin(); it != faceCache.end(); ++count, ++it)
						{
							if (count == id)
							{
								FT_Done_Face(it->second);
								faceCache.erase(it);
								break;
							}
						}
					}
					faceCache.insert(std::pair<size_t, FT_Face>(hashCode, nullptr));
					ftError = FT_New_Face(fontLibrary, fileName, 0, &faceCache[hashCode]);
					if (!!ftError)
					{
						faceCache.erase(hashCode);
					}
				}
			}

			FT_Face operator [] (const char* font) aNOEXCEPT
			{
				size_t hashCode = hashCStringLiteral(font);
				if (faceCache.find(hashCode) == faceCache.end())
					addFace(font);
				return faceCache[hashCode];
			}
		private:
			size_t limit;
			//{hash of directory path, pointer to face on that directory}
			std::unordered_map<size_t, FT_Face> faceCache;
		};
	}

	struct FontBitmap
	{
		explicit FontBitmap()
		{
			buffer = nullptr;
		}
		~FontBitmap()
		{
			aSAFE_DELETE(buffer);
		}
		unsigned char* buffer;
		size_t width;
		size_t height;
		size_t penX;
		size_t penY;
		size_t size;
	};

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
		;
	public:
		typedef std::unique_ptr<FontBitmap[]> FontResults;
		~FontEngine()
		{
			FT_Done_FreeType(fontLibrary);
		}
		FontResults getFontBitmap(const char* fontName,
			const wchar_t* text,
			int size,
			int resX, int resY) aNOEXCEPT
		{
			;
		}

		//fontName where the font exist.
		//text the text gonna be rendered
		//size font size
		//resX resY resolution of the font.
		FontResults getRawFontBitmap(const char* fontName,
			const wchar_t* text,
			int size,
			int resX, int resY) aNOEXCEPT
		{
			int penX = 0, penY = 0;
			int len = wcslen(text);
			FT_Face face = faceCache[fontName];
			std::unique_ptr<FontBitmap[]> result(new FontBitmap[len]);
			ftError = FT_Set_Char_Size(face, 0, size * 64, resX, resY);
			
			for (int i = 0; i < len; i++)
			{
				FT_UInt glyph_index;
				FT_GlyphSlot slot = face->glyph;

				glyph_index = FT_Get_Char_Index(face, text[i]);
				ftError = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
				if (ftError)
					continue;
				ftError = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
				if (ftError)
					continue;

				auto ret = result[i];
				ret.size = size;
				ret.width = slot->bitmap.width;
				ret.height = slot->bitmap.rows;
				ret.penX = penX + slot->bitmap_left;
				ret.penY = penY + slot->bitmap_top;
				std::memcpy(ret.buffer, slot->bitmap.buffer, ret.width * ret.height);
				ret.buffer = slot->bitmap.buffer;
				penX += slot->advance.x >> 6;
				penY += slot->advance.y >> 6;
			}
			return std::move(result);
		}
	};
}
#define FONT (aMazing::FontEngine::getInstance())
