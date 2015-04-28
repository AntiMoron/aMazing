#pragma once
#include"../../common/CommonDef.hpp"

namespace aMazing
{
	template<typename T>
	class aRandomAccessIterator
	{
	private:
		friend class CommonContainer;
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
		aRandomAccessIterator(aRandomAccessIterator<T>&& other) aNOEXCEPT
		{
			_p = other._p;
		}
		aRandomAccessIterator(const aRandomAccessIterator<T>& other) aNOEXCEPT
		{
			_p = other._p;
		}
		explicit aRandomAccessIterator(aRandomAccessIterator<const T>&& other) aNOEXCEPT
		{
			_p = other._p;
		}
		explicit aRandomAccessIterator(const aRandomAccessIterator<const T>& other) aNOEXCEPT
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
		


		T* operator &()const aNOEXCEPT
		{
			return _p;
		}
		T& operator *() const aNOEXCEPT
		{
			return *p;
		}
	private:
		T* _p;
	};
}