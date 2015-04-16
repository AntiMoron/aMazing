#pragma once
#include"../lang/aNoCopyable.hpp"
#include"aAtomicObject.hpp"

namespace aMazing
{
	template<typename T>
	class aThreadSafeSingleton : public aNoCopyable
	{
	private:
		//make sure every instance is initialized at the time when it's defined.
		struct InstanceSafe
		{
			InstanceSafe()
			{
				aThreadSafeSingleton<T>::getInstance();
			}
			//This function does nothing but to ensure instance is initialized.
			inline void nothing() const{}
		};
		static InstanceSafe safer;
	public:
		static T& getInstance()
		{
			static T inst;
			safer.nothing();
			return inst;
		}
	private:
		static T& instance;
	};

	template<typename T>
	typename aThreadSafeSingleton<T>::InstanceSafe aThreadSafeSingleton<T>::safer;
	template<typename T>
	T& aThreadSafeSingleton<T>::instance = aThreadSafeSingleton::getInstance();
}