#pragma once
#include"CommonDef.hpp"
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

	template<typename T>
	T aMin(T a, T b)
	{
		if (a < b)
			return a;
		return b;
	}

	template<typename T>
	T aMax(T a,T b)
	{
		if (b < a)
			return a;
		return b;
	}
	/**
	*@brief To judge whether a type T is numeric type.
	*/
	template<typename T>
	struct aIsNumeric
	{
		const static bool value = std::is_floating_point<T>::value ||
		std::is_integral<T>::value;
	};

	/*
	The deleter for COM components.
	*/
	template<class T>
	struct aComDelete
	{
		// default deleter for unique_ptr
		aCONSTEXPR aComDelete() aNOEXCEPT{}

		template<class U,
		class = typename std::enable_if<std::is_convertible<U*, T*>::value, void>::type>
			aComDelete(const aComDelete<U>&) aNOEXCEPT
		{	// construct from another default_delete
		}

		void operator()(T* ptr) const aNOEXCEPT
		{	// delete a pointer
			static_assert(0 < sizeof (T),
			"can't delete an incomplete type");
			aSAFE_RELEASE(ptr);
		}
	};
}
