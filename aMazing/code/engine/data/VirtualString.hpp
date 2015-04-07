#pragma once
#include<string>
#include"../exception/ParamException.hpp"
#include"../../common/CommonDef.hpp"
#include"../../common/CommonTemplates.hpp"

namespace aMazing
{
	template<typename type>
	class VirtualString_t
	{
		template<typename itType>
		class vsIterator : public std::iterator<std::random_access_iterator_tag, itType>
		{
		private:
			itType* p;
		public:
			vsIterator(itType* _p = nullptr) : p(_p){}
			vsIterator<itType> operator + (size_t i) const
			{
				return p + i;
			}
			vsIterator<itType> operator - (size_t i) const
			{
				return p - i;
			}
			itType operator * () const
			{
				return *p;
			}
			vsIterator<itType>& operator ++ ()
			{
				++p;
				return *this;
			}
			vsIterator<itType>& operator -- ()
			{
				--p;
				return *this;
			}
			vsIterator<itType>& operator += (size_t i)
			{
				p += i;
				return *this;
			}
			vsIterator<itType>& operator -= (size_t i)
			{
				p -= i;
				return *this;
			}
			friend bool operator != (const vsIterator& lhs, const vsIterator& rhs)
			{
				return lhs.p != rhs.p;
			}
			friend bool operator == (const vsIterator& lhs, const vsIterator& rhs)
			{
				return lhs.p == rhs.p;
			}
		};
	public:
		typedef vsIterator<type> iterator;
		typedef vsIterator<const type> const_iterator;
		typedef size_t size_type;
		/*
		@brief VirtualString imitates a period of type 'type' from a real string as a string.
		Via generating VirtualString can accelerate substring procedure(s).
		@param st  the start position of string
		@param length the length of substring. 
		If the requested substring extends past the end of the string.the return substring is[pos,size())
		*/
		VirtualString_t(const std::basic_string<type>& cppString, size_t st, size_t length)
		{
			if (aSTL_OUT_OF_RANGE(st, cppString))
			{
				throw ParamException("position(s) out of range");
			}
			if (aSTL_OUT_OF_RANGE(st + length - 1, cppString))
			{
				vSize = cppString.length() - st;
			}
			else
				vSize = length;
			rawString = &cppString[st];
		}
		VirtualString_t() = delete;
		VirtualString_t(typename std::remove_reference<std::basic_string<type> >::type&&, ...) = delete;
		//RVO
		const const_iterator& begin() const
		{
			return rawString;
		}
		const const_iterator& end() const
		{
			return rawString + vSize;
		}
		const size_t size() const
		{
			return vSize;
		}
		const type& operator [] (size_t index) const
		{
			return rawString[index];
		}
		friend std::ostream& operator << (std::ostream &o, const VirtualString_t<type> &a)
		{
			for (size_t i = 0; i < a.size(); i++)
			{
				o << a.rawString[i];
			}
			return o;
		}
	private:
		const type* rawString;
		size_t vSize;
	};
}
