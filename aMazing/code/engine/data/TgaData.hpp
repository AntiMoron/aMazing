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

		void display()
		{
			printf("TgaLoader [%p] :\nid:%d\n"
				"colorMapType:%d\n"
				"imageType:%d\n"
				"ColorMapSpecification:\n"
				"\tfirst entry index:%d\n"
				"\tcolor map lenth:%d\n"
				"\tcolor map Entry Size:%d\n"
				"Image Specification\n"
				"\tX-origin of Image:%d\n"
				"\tY-origin of Image:%d\n"
				"\tImage Width : %d\n"
				"\tImage Height : %d\n"
				"\tPixel Depth : %d\n"
				"\tImage Descriptor : %d\n",
				this,
				id,
				colorMapType,
				imageType,
				firstEntryIndex,
				colorMapLenth,
				colorMapEntrySize,
				xOrigin,
				yOrigin,
				width,
				height,
				pixelDepth,
				imageDescriptor
				);
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
