#pragma once
#include<atomic>

namespace aMazing
{
	namespace reserve
	{
		template<typename T>
		class aAtomicWrapper
		{
		public:
			typedef std::atomic<typename enable_if<is_fundamental<T>::value, T>::type> type;
		};
	}

	template<typename T>
	using aAtomicObject = typename reserve::aAtomicWrapper<T>::type;
}
