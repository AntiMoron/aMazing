#pragma once
#include<string>
#include<vector>
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
		
		VirtualString_t(const VritualString_t<type>& vString, size_t st, size_t length)
		{
			if (aSTL_OUT_OF_RANGE(st, cppString))
			{
				throw ParamException("position(s) out of range");
			}
			if (aSTL_OUT_OF_RANGE(st + length - 1, cppString))
			{
				vSize = vString.length() - st;
			}
			else
				vSize = length;
			rawString = &vString[st];
		}

		VirtualString_t() = delete;
		VirtualString_t(typename std::remove_reference<std::basic_string<type> >::type&&, size_t,size_t) = delete;
		
		//Get the begin itor of the virtual substr.
		const const_iterator& begin() const
		{
			//RVO
			return rawString;
		}
		//Get the end itor of the virtual substr.
		const const_iterator& end() const
		{
			//RVO
			return rawString + vSize;
		}

		const size_t length() const
		{
			return vSize;
		}
		const size_t size() const
		{
			return vSize;
		}
		const type& operator [] (size_t index) const
		{
			return rawString[index];
		}

		VirtualString_t<type> subString(size_t st, size_t len = -1) const throw (ParamException)
		{
			return VirtualString_t<type>(rawString, st, len);
		}
		//split the string by character.
		std::vector<VirtualString_t<type> > splitString(char ch) const
		{
			std::vector<VirtualString_t<type> > result;
			int start = 0;
			int i = start;
			for (; i < str.vSize; i++)
			{
				if (str[i] == ch)
				{
					result.push_back(subString(start, i - start));
					start = i + 1;
					i += 1;
				}
			}
			if (i >= str.length())
			{
				i = str.length();
				result.push_back(subString(start, i - start));
			}
			return result;
		}
		//split the string by regular expression.
		std::vector<VirtualString_t<type> > splitString(const char* reg) const
		{
			std::vector<VirtualString_t<type> > result;
			return result;
		}
		/*
		get the stl string of this virtual string
		*/
		std::basic_string<type> toStlString()
		{
			std::basic_string<type> result;
			for (size_t cur = 0; cur < vSize; ++cur)
			{
				result += rawString[cur];
			}
			return result;
		}

		/*
		Trim out blank characters on both sides.
		*/
		const VirtualString_t<type>& trim() const
		{
			size_t startPos = 0;
			size_t endPos = vSize - 1;
			while (startPos < vSize && aMazing::isBlank((*this)[startPos]))
			{
				startPos++;
			}
			while (endPos < vSize && aMazing::isBlank((*this)[endPos]))
			{
				endPos--;
			}
			endPos++;
			if (startPos >= endPos)
			{
				vSize = 0;
				return *this;
			}
			rawString += startPos;
			vSize = endPos - startPos;
			return *this;
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

	typedef VirtualString_t<char> VirtualString;
	typedef VirtualString_t<wchar_t> wVirtualString;
}
