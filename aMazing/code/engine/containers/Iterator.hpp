#pragma once
#include <type_traits>
#include "../../common/CommonDef.hpp"

namespace aMazing
{
	template<typename T>
	class aCommonIterator
	{
	};

	template<typename T>
	class aRandomAccessIterator : public aCommonIterator<T>
	{
	private:
		typedef typename std::remove_const<T>::type rawType;
		friend class aRandomAccessIterator<rawType>;
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

		operator aRandomAccessIterator<const rawType>()
		{
			return aRandomAccessIterator<const rawType>(_p);
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
		size_t operator - (const aRandomAccessIterator<T>& other) const aNOEXCEPT
		{
			return _p - other._p;
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
		T* operator->() const aNOEXCEPT
		{
			return _p;
		}
	protected:
		T* _p;
	};
}
