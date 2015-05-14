#ifndef _TGALOADER_HPP__
#define _TGALOADER_HPP__

#include<cstdio>
#include<string>
#include<cstring>
#include<memory>
#include<exception>
#include<algorithm>
#include"TgaData.hpp"
#include"../../common/CommonUtil.hpp"

namespace aMazing
{
	namespace TGA
	{
		class TgaLoader
		{
		private:
			TgaLoader(){}
			~TgaLoader(){}
		public:
			//Use this method to get a tgaLoader
			static TgaLoader& getLoader()
			{
				static TgaLoader instance;
				return instance;
			}
			//LoadTgaFile
			bool loadFile(const char* filename, TgaData* pOut)
			{
				if (isSuffix(filename, ".tga") == false && isSuffix(filename, ".TGA") == false)
				{
					printf("can't open file '%s' at 'LoadFile' function.\r\n", filename);
					throw bad_file_exception("this is not a .tga file.\r\n");
				}

				FILE* file_ptr = nullptr;
				file_ptr = fopen(filename, "rb");
				if (nullptr == file_ptr)
				{
					printf("can't open file '%s' at 'LoadFile' function.\r\n", filename);
					throw bad_file_exception("file not exist");
					return false;
				}

				unsigned char temData[18];
				fread(&temData, sizeof(unsigned char), 18, file_ptr);
				pOut->id = temData[0];
				pOut->colorMapType = temData[1];
				pOut->imageType = temData[2];
				//color map specification
				pOut->firstEntryIndex = *((unsigned short*)&temData[3]);
				pOut->colorMapLenth = *((unsigned short*)&temData[5]);
				pOut->colorMapEntrySize = temData[7];
				//image specification
				pOut->xOrigin = *((unsigned short*)&temData[8]);
				pOut->yOrigin = *((unsigned short*)&temData[10]);
				pOut->width = *((unsigned short*)&temData[12]);
				pOut->height = *((unsigned short*)&temData[14]);
				pOut->pixelDepth = temData[16];
				pOut->imageDescriptor = temData[17];

				if (pOut->imageType != 10)
				{
					if (file_ptr != nullptr)
					{
						fclose(file_ptr);
						file_ptr = nullptr;
					}
					throw bad_file_exception("File is not RLE-TrueColor File.\n");
					return false;
				}

				if (pOut->id != 0)
				{
					printf("Image Ids:\n");
					unsigned char* imageId = new unsigned char[pOut->id];
					fread(imageId, sizeof(unsigned char), pOut->id, file_ptr);
					for (int i = 0; i<pOut->id; i++)
					{
						printf("%d ", imageId[i]);
					}
					if (imageId != nullptr)
					{
						delete[] imageId;
						imageId = nullptr;
					}
				}
				else
				{
					printf("No Image Id.\n");
				}
				// in the test tga file I used ,there's no color map.
				//So I didn't process the color map data temporarily.
				if (pOut->colorMapType != 0)
				{
					//get min(colorMapEntrySize / 3,8)
					int colorBitField = pOut->colorMapEntrySize / 3;
					if (colorBitField > 8)
					{
						colorBitField = 8;
					}
				}
				else
				{
					printf("There is no color map.\n");
				}

				short bytesPerPixel = pOut->pixelDepth / 8;
				std::size_t pixelCount = (unsigned long)(pOut->width) * (unsigned long)(pOut->height);
				pOut->pColor = new color4i[pixelCount];
				unsigned char temColorData[4];
				// read the true-type image data.
				std::size_t processIndex = 0;
				while (processIndex < pixelCount)
				{
					unsigned char runLengthField;
					fread(&runLengthField, sizeof(runLengthField), 1, file_ptr);
					bool isRunLengthData = (0x80 & runLengthField) != 0;
					unsigned char successivePixelCount = 0x7f & runLengthField;
					successivePixelCount += 1;

					if (isRunLengthData == true)
					{
						fread(temColorData, sizeof(unsigned char), bytesPerPixel, file_ptr);
						while (successivePixelCount--)
						{
							if (processIndex >= pixelCount)
							{
								break;
							}

							pOut->pColor[processIndex].b = temColorData[0];
							if (bytesPerPixel >= 1)
								pOut->pColor[processIndex].g = temColorData[1];
							else
								pOut->pColor[processIndex].g = unsigned char(0);
							if (bytesPerPixel >= 2)
								pOut->pColor[processIndex].r = temColorData[2];
							else
								pOut->pColor[processIndex].r = unsigned char(0);
							if (bytesPerPixel >= 3)
								pOut->pColor[processIndex].a = temColorData[3];
							else
								pOut->pColor[processIndex].a = unsigned char(0);
							++processIndex;
						}
					}
					else
					{
						while (successivePixelCount--)
						{
							if (processIndex >= pixelCount)
							{
								break;
							}
							fread(temColorData, sizeof(unsigned char), bytesPerPixel, file_ptr);
							pOut->pColor[processIndex].b = temColorData[0];
							if (bytesPerPixel >= 1)
								pOut->pColor[processIndex].g = temColorData[1];
							else
								pOut->pColor[processIndex].g = unsigned char(0);
							if (bytesPerPixel >= 2)
								pOut->pColor[processIndex].r = temColorData[2];
							else
								pOut->pColor[processIndex].r = unsigned char(0);
							if (bytesPerPixel >= 3)
								pOut->pColor[processIndex].a = temColorData[3];
							else
								pOut->pColor[processIndex].a = unsigned char(0);
							++processIndex;
						}
					}
				}

				printf("Image is scaned ordering by %d %d %d\n",
					pOut->imageDescriptor,
					pOut->imageDescriptor >> 3 & 0x1, 
					pOut->imageDescriptor >> 4 & 0x1);
				if (0 == (pOut->imageDescriptor >> 4 & 0x1))
				{
					for (int i = 0; i<pOut->height / 2; i++)
					{
						for (int j = 0; j < pOut->width; j++)
						{
							aSwap<color4i>(pOut->pColor[std::size_t(pOut->width) * i + j],
								pOut->pColor[std::size_t(pOut->width) * std::size_t(pOut->height - i - 1) + j]);
						}
					}
				}
				if (1 == (pOut->imageDescriptor >> 3 & 0x1))
				{
					for (int i = 0; i<pOut->width / 2; i++)
					{
						for (int j = 0; j < pOut->height; j++)
						{
							aSwap<color4i>(pOut->pColor[std::size_t(pOut->height) * i + j],
								pOut->pColor[std::size_t(pOut->height) * std::size_t(pOut->width - i - 1) + j]);
						}
					}
				}

				if (nullptr != file_ptr)
				{
					fclose(file_ptr);
					file_ptr = nullptr;
				}
				return true;
			}

			class bad_file_exception : public std::exception
			{
			public:
				bad_file_exception(const char* err_str = nullptr)
				{
					if (nullptr == err_str)
					{
						error_msg = "bad_file_exception.\r\n";
						return;
					}
					error_msg = err_str;
				}
				~bad_file_exception(){}

				const char* what() const
				{
					return error_msg.c_str();
				}
			private:
				std::string error_msg;
			};
			//judge whether a file path is tga file
			bool judgeTga(const char* path)
			{
				if (isSuffix(path,"TGA") ||
					isSuffix(path,"tga") )
				{
					return true;
				}
				return false;
			}
		private:
			//judge string@suf whether is the suffix of string@src
			bool isSuffix(const std::string& src, const std::string& suf)
			{
				size_t lenSrc = src.length();
				size_t lenSuf = suf.length();
				if (lenSrc < lenSuf)
					return false;
				if (lenSrc == lenSuf)
					return src == suf;
				//"abcdef" "abc"
				size_t sufCur = 0;
				for (size_t srcCur = lenSrc - lenSuf; srcCur < lenSrc; ++srcCur, ++sufCur)
				{
					if (src[srcCur] != suf[sufCur])
					{
						return false;
					}
				}
				return true;
			}
		};
	}
}
#define TGALOAD(path,out) (TGA::TgaLoader::getLoader().loadFile(path,out))
#endif // _TGALOADER_H__
