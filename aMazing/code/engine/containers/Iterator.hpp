#pragma once
#include <type_traits>
#include "../../common/CommonDef.hpp"

namespace aMazing
{
	template<typename Type>
	class aRandomAccessIterator : public std::iterator<std::random_access_iterator_tag, Type>
	{
	private:
		typedef typename std::remove_const<Type>::type rawType;
		typedef aRandomAccessIterator<Type> self_type;
		friend class aRandomAccessIterator<rawType>;
	public:
		//default ctor
		aRandomAccessIterator() aNOEXCEPT
		{
			_p = nullptr;
		}
		aRandomAccessIterator(Type* p) aNOEXCEPT
		{
			_p = p;
		}

		void swap(self_type& other)
		{
			aSwap(_p,other._p);
		}

		operator aRandomAccessIterator<const rawType>() aNOEXCEPT
		{
			return aRandomAccessIterator<const rawType>(_p);
		}

		self_type& operator ++ () aNOEXCEPT
		{
			++_p;
			return *this;
		}
		self_type& operator -- () aNOEXCEPT
		{
			--_p;
			return *this;
		}
		self_type& operator ++ (int)aNOEXCEPT
		{
			_p++;
			return *this;
		}
		self_type& operator -- (int)aNOEXCEPT
		{
			_p--;
			return *this;
		}

		self_type& operator += (size_t offset) aNOEXCEPT
		{
			_p += offset;
			return *this;
		}
		self_type& operator -= (size_t offset) aNOEXCEPT
		{
			_p -= offset;
			return *this;
		}

		self_type operator + (size_t offset) const aNOEXCEPT
		{
			return _p + offset;
		}
		self_type operator - (size_t offset) const aNOEXCEPT
		{
			return _p - offset;
		}

		//Return the distance between two iterator.
		size_t operator - (const self_type& other) const aNOEXCEPT
		{
			return _p - other._p;
		}

		bool operator == (const self_type& other) const aNOEXCEPT
		{
			return _p == other._p;
		}
		bool operator != (const self_type& other) const aNOEXCEPT
		{
			return _p != other._p;
		}
		bool operator < (const self_type& other) const aNOEXCEPT
		{
			return _p < other._p;
		}
		bool operator >(const self_type& other) const aNOEXCEPT
		{
			return _p > other._p;
		}
		bool operator <= (const self_type& other) const aNOEXCEPT
		{
			return _p <= other._p;
		}
		bool operator >= (const self_type& other) const aNOEXCEPT
		{
			return _p >= other._p;
		}
		Type& operator *() const aNOEXCEPT
		{
			return *_p;
		}
		Type* operator->() const aNOEXCEPT
		{
			return _p;
		}
	protected:
		Type* _p;
	};
}
