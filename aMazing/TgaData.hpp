#ifndef _TGADATA_HPP__
#define _TGADATA_HPP__

namespace TGA
{
	class color4i
	{
	public:
		unsigned char r, g, b, a;
	};
	class TgaData
	{
	public:
		TgaData()
		{
			pColor = nullptr;
		}
		~TgaData()
		{
			if (pColor != nullptr)
			{
				delete[] pColor;
				pColor = nullptr;
			}
		}
		unsigned short getWidth()const
		{
			return width;
		}
		unsigned short getHeight()const
		{
			return height;
		}
		unsigned char getPixelDepth()const
		{
			return pixelDepth;
		}
		//get the color data pointer
		TGA::color4i* getColDataPtr() const
		{
			return pColor;
		}
	private:
		friend class TgaLoader;
		//
		unsigned char id;
		unsigned char colorMapType;
		unsigned char imageType;
		//color map specification
		unsigned short firstEntryIndex;
		unsigned short colorMapLenth;
		unsigned char colorMapEntrySize;
		//image specification
		unsigned short xOrigin;
		unsigned short yOrigin;
		unsigned short width;
		unsigned short height;
		unsigned char pixelDepth;
		unsigned char imageDescriptor;
		//color Data
		color4i* pColor;
	};
}

#endif // _TGADATA_HPP__
