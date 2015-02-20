#pragma once
#include<type_traits>
#include<vector>


namespace aMazing
{
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

/*
*@brief serialize a data
*@param T& data The data that need to be serialized
*@return const std::vector<unsigned char>
*/
template<typename T>
const std::vector<unsigned char> getDataSerialization(T& data)
{
	auto pData = reinterpret_cast<unsigned char*>(getAddressOf(data));
	std::vector<unsigned char> result;
	result.resize(sizeof(T));
	for (std::vector<unsigned char>::size_type i = 0; i < result.size(); i++)
	{
		result[i] = pData[i];
	}
	return result;
}
/*
*@brief Decode from byte stream as type T
*@param const std::vector<unsigned char>& data The data need to be decoded
*@return const T
*/
template<typename T>
const T getDataFromSerialization(const std::vector<unsigned char>& data)
{
	auto pData = reinterpret_cast<T*>(&data[0]);
	return *pData;
}

}
