#pragma once
#include<memory>
#include<unordered_map>
#include"fontglobal.hpp"
#include"../../util/randomgen.hpp"
#include"../containers/Directory.hpp"
#include"../../common/CommonDef.hpp"
#include"../../common/CommonFunction.hpp"

namespace aMazing
{
	namespace detail
	{
		class FaceCache
		{
		public:
			FaceCache(size_t lim = 1000)
			{
				limit = lim;
			}
			bool addFace(const char* fileName)
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
						//abort the operation
						return false;
					}
					FT_Face aFace;
					aDBG(fontLibrary);
					ftError = FT_New_Face(fontLibrary, fileName, 0, &aFace);
					if (!!ftError)
					{
						aDBG("freetype failed with error code£º" << ftError);
						faceCache.erase(hashCode);
						return false;
					}
					else
					{
						faceCache.insert(std::pair<size_t, FT_Face>(hashCode, aFace));
					}
				}
				return true;
			}

			FT_Face operator [] (const char* fontName) aNOEXCEPT
			{
				aString _fileName = transformSplashes(fontName);
				_fileName = minimizeDirectory(_fileName);
				size_t hashCode = hashCStringLiteral(_fileName.c_str());
				if (faceCache.find(hashCode) == faceCache.end())
				{
					if (!addFace(_fileName.c_str()))
						return faceCache[0];
				}
				return faceCache[hashCode];
			}
		private:
			size_t limit;
			//{hash of directory path, pointer to face on that directory}
			std::unordered_map<size_t, FT_Face> faceCache;
		};
	}
}
