#pragma once
#include<type_traits>

namespace aMazing
{
	template<typename Type>
	class aIsCharLike
	{
	public:
		const static bool value = std::is_pod<Type>::value && 
			(!std::is_array<Type>::value);
	};
}
