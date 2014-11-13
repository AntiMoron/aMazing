#pragma once

//aMazing swap
template<typename T>
void aSwap(T& a, T& b)
{
	T t = a;
	a = b;
	b = t;
}
