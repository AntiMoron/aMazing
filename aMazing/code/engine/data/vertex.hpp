#pragma once
#include"../../common/CommonDxSupport.hpp"
namespace aMazing
{
	//POD vertex for static model
	struct Vertex
	{
	public:
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 texture;
	};

	//POD vertex for skinning animation model
	struct SkinVertex
	{
	public:
		//must call this function to modify bone Index.
		//!!!do not modify value of boneIndices directly.
		void setBoneIndex(unsigned int slot,unsigned char value)
		{
#ifdef BONE_UINT4_INDEX
			boneIndices[slot] = value;
#else
			auto uiValue = (unsigned int)value << (slot * 8);
			switch (slot)
			{
			case 0:
				boneIndices &= 0xffffff00; break;
			case 1:
				boneIndices &= 0xffff00ff; break;
			case 2:
				boneIndices &= 0xff00ffff; break;
			case 3:
				boneIndices &= 0x00ffffff; break;
			}
			boneIndices |= uiValue;
#endif
		}
		XMFLOAT4 weights;
		XMFLOAT4 position;
		XMFLOAT4 normal;
		XMFLOAT4 texture;
#ifdef BONE_UINT4_INDEX
		unsigned int boneIndices[4];
#else
		unsigned int boneIndices;
#endif
	};
}
