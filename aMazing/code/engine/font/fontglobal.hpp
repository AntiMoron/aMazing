#pragma once
#include"../../common/CommonDef.hpp"
#include<ft2build.h>
#include<memory>
#include FT_FREETYPE_H 

#if defined(_DEBUG)
#pragma comment(lib,"freetype253_D.lib")
#else
#pragma comment(lib,"freetype253.lib")
#endif

namespace aMazing
{
	namespace detail
	{
		extern FT_Error ftError;
		extern FT_Library fontLibrary;
	}

	struct FontBitmap
	{
		explicit FontBitmap()
		{
			buffer = nullptr;
		}
		~FontBitmap()
		{
			aSAFE_DELETE_ARRAY(buffer);
		}
		unsigned char* buffer;
		size_t width;
		size_t height;
		int bitmapLeft;
		int bitmapTop;
		int nextPenX;
		int nextPenY;
		int outlineX;
		int outlineY;
		size_t size;
	};
	typedef FontBitmap* FontResults;
}
