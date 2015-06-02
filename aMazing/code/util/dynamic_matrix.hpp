#pragma once
#include"../common/CommonDef.hpp"
#include<memory>
namespace aMazing
{
	template<typename T>
	class DynamicMatrix
	{
		static_assert(std::is_pod<T>::value,"Template argument must be pod type.");
	public:
		explicit DynamicMatrix() = delete;
		explicit DynamicMatrix(int w, int h) aNOEXCEPT
		{
			data.reset(new Type[w * h]);
			width = w;
			height = h;
		}
		inline Type* operator [](int index) const aNOEXCEPT
		{
			return data[index * height];
		}
	private:
		int width;
		int height;
		std::unique_ptr<Type[]> data;
	};
}
