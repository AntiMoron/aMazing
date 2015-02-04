#pragma once
#include<type_traits>
/*
 *aMazing swap
 *swap two reference value
*/
template<typename T>
void aSwap(T& a, T& b)
{
	T t = a;
	a = b;
	b = t;
}


/*
 *aMazing addressof
 *@param T  template argument
 *@return T*   get the pointer of an reference value of type T
*/
template<typename T>
T* aAddressof(T& lval)
{
	return reinterpret_cast<T*>(&const_cast<T>(
		reinterpret_cast<const volatile char&>(lval)));
}

