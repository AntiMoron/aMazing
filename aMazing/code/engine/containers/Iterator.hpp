#pragma once
#include <type_traits>
#include "../../common/CommonDef.hpp"

namespace aMazing
{
	template<typename T>
	class aRandomAccessIterator
	{
	private:
		typedef typename std::remove_reference<T>::type non_ref_type;
		typedef typename std::remove_const<non_ref_type>::type rawType;
	public:
		//default ctor
		aRandomAccessIterator() aNOEXCEPT
		{
			_p = nullptr;
		}
		aRandomAccessIterator(T* p) aNOEXCEPT
		{
			_p = p;
		}
		aRandomAccessIterator(aRandomAccessIterator<rawType>&& other) aNOEXCEPT
		{
			_p = other._p;
		}
		aRandomAccessIterator(const aRandomAccessIterator<rawType>& other) aNOEXCEPT
		{
			_p = other._p;
		}
		aRandomAccessIterator(aRandomAccessIterator<const rawType>&& other) aNOEXCEPT
		{
			_p = other._p;
		}
		aRandomAccessIterator(const aRandomAccessIterator<const rawType>& other) aNOEXCEPT
		{
			_p = other._p;
		}
		aRandomAccessIterator<T>& operator ++ () aNOEXCEPT
		{
			++_p;
			return *this;
		}
		aRandomAccessIterator<T>& operator -- () aNOEXCEPT
		{
			--_p;
			return *this;
		}
		aRandomAccessIterator<T>& operator ++ (int) aNOEXCEPT
		{
			_p++;
			return *this;
		}
		aRandomAccessIterator<T>& operator -- (int) aNOEXCEPT
		{
			_p--;
			return *this;
		}
		aRandomAccessIterator<T> operator + (size_t offset) const aNOEXCEPT
		{
			return aRandomAccessIterator<T>(_p + offset);
		}
		aRandomAccessIterator<T> operator - (size_t offset) const aNOEXCEPT
		{
			return aRandomAccessIterator<T>(_p - offset);
		}
		//Return the distance between two iterator.
		size_t operator - (aRandomAccessIterator<T>& other) const aNOEXCEPT
		{
			return p - other._p;
		}

		bool operator == (const aRandomAccessIterator<T>& other) const aNOEXCEPT
		{
			return _p == other._p;
		}
		bool operator != (const aRandomAccessIterator<T>& other) const aNOEXCEPT
		{
			return _p != other._p;
		}
		bool operator < (const aRandomAccessIterator<T>& other) const aNOEXCEPT
		{
			return _p < other._p;
		}
		bool operator >(const aRandomAccessIterator<T>& other) const aNOEXCEPT
		{
			return _p > other._p;
		}
		bool operator <= (const aRandomAccessIterator<T>& other) const aNOEXCEPT
		{
			return _p <= other._p;
		}
		bool operator >= (const aRandomAccessIterator<T>& other) const aNOEXCEPT
		{
			return _p >= other._p;
		}
		T& operator *() const aNOEXCEPT
		{
			return *_p;
		}
	private:
		T* _p;
	};
}
