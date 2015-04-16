#pragma once
#include<type_traits>
#include<atomic>

namespace aMazing
{
	namespace reserve
	{
		template<typename T>
		class aAtomicWrapper
		{
		public:
			typedef std::atomic<typename std::enable_if<std::is_fundamental<T>::value, T>::type> type;
		};
	}

	template<typename T>
	using aAtomicObject = typename reserve::aAtomicWrapper<T>::type;
}
