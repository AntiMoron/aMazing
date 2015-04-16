#pragma once

#include"aAtomicObject.hpp"

namespace aMazing
{
	template<typename T>
	class aThreadSafeSingleton
	{
	public:
		static T& getInstance()
		{
			if (!instance)
			{
				instance = new T;
			}
			return *instance;
		}
	private:
		static T* instance;
	};
}