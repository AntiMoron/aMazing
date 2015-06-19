#pragma once
#include"../../common/CommonDxSupport.hpp"
#include"../../common/CommonHelper.hpp"
#include"../../common/CommonDef.hpp"
#include"../exception/FailureException.hpp"
#include<unordered_map>

namespace aMazing
{
	enum VertexType
	{
		VERTEX_TYPE,
		SKIN_VERTEX_TYPE,
	};
	namespace detail
	{
		struct VirtualVertexBase{};

		template<typename Vertex>
		class aHasStaticInputLayout
		{
		private:
			template<typename C>
			const static yes_type& test(typename std::enable_if<std::is_const<decltype(C::input_layout)>::value, int*>::type);
			template<typename C>
			const static no_type& test(...);
		public:
			const static bool value = (sizeof(test(nullptr)) == sizeof(yes_type));
		};
	
		namespace
		{
			static std::unordered_map<std::string, VertexType> vertexTypeMapper = {
				{ "Vertex", VERTEX_TYPE },
				{ "SkinVertex", SKIN_VERTEX_TYPE }
			};
		}

		extern VertexType getVertexTypeByLayoutName(const std::string& layoutName)
			throw(FailureException);
	}

	//Must be base class of all vertex types.
	//!!!note : Need to contain type input_layout.
	template<typename Vertex, bool Enable = true>
	struct BaseVertex : detail::VirtualVertexBase{};

	template<typename Vertex>
	struct BaseVertex<Vertex, false>;

	struct Vertex : BaseVertex<Vertex>
	{
	public:
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 texture;

		const static D3D11_INPUT_ELEMENT_DESC input_layout[3];
	};


	//vertex for skinning animation model
	struct SkinVertex : BaseVertex<SkinVertex>
	{
	public:
		//must call this function to modify bone Index.
		//!!!do not modify value of boneIndices directly.
		void setBoneIndex(unsigned int slot, unsigned char value)
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
		const static D3D11_INPUT_ELEMENT_DESC input_layout[5];
	};
}
