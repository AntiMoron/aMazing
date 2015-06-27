#pragma once
#include"../../../common/CommonDef.hpp"
#include"../../../common/CommonDxSupport.hpp"

namespace aMazing
{
	template<typename Type>
	class BaseMesh
	{
	public:
		BaseMesh()
		{
			bIsInited = false;
			indicesBuffer = nullptr;
			verticesBuffer = nullptr;
		}
		~BaseMesh()
		{
			aSAFE_RELEASE(verticesBuffer);
			aSAFE_RELEASE(indicesBuffer);
		}
		bool isInited() const
		{
			return bIsInited;
		}
	protected:
		volatile bool bIsInited;
		std::size_t verticesCount;
		std::size_t indicesCount;
		ID3D11Buffer* indicesBuffer;
		ID3D11Buffer* verticesBuffer;
	};
}
