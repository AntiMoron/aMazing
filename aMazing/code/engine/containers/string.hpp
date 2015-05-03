#pragma once
#include"../../common/CommonDef.hpp"
#include"../exception/MemoryException.hpp"
#include"check/CharLike.hpp"
#include"./vector.hpp"

namespace aMazing
{
	template<typename Type,typename Allocator = std::allocator<Type> >
	class aString_t
	{
	private:
		typedef aString_t<Type,Allocator> self_type;
		typedef typename std::remove_const<typename std::remove_reference<Type>::type>::type rawType;
	public:
		typedef size_t size_type;
		typedef Allocator allocator_type;
		typedef typename aVector<Type,Allocator>::iterator iterator;
		typedef typename aVector<Type, Allocator>::const_iterator const_iterator;

		aString_t(){}
		template<typename EType, size_type cnt>
		aString_t(EType (&arr)[cnt])
		{
			resize(cnt - 1);
			for (size_type cur = 0; cur < cnt; ++cur)
			{
				mData[cur] = arr[cur];
			}
		}
		~aString_t(){}
		bool empty() const aNOEXCEPT
		{
			return mData.empty();
		}
		void clear() aNOEXCEPT
		{
			mData.clear();
		}
		void resize(size_type newSize) aNOEXCEPT
		{
			mData.resize(newSize);
		}
		size_type size() const aNOEXCEPT
		{
			return mData.size();
		}
		size_type length() const aNOEXCEPT
		{
			return mData.size();
		}
		size_type capacity() const aNOEXCEPT
		{
			return mData.capacity();
		}
		aMazing::aVector<self_type > splitString(char ch) const aNOEXCEPT;
		//aMazing::aVector<aString_t<Type> > splitString(Regex&&) const aNOEXCEPT;
		bool trim() aNOEXCEPT
		{
			return true;
		}
		void push_back(Type ch) aNOEXCEPT
		{
			mData.push_back(ch);
		}
		void pop_back() aNOEXCEPT
		{
			mData.pop_back();
		}
		void insert(iterator pos, Type ch) aNOEXCEPT
		{
			mData.insert(pos, ch);
		}
		void insert(const_iterator pos, Type ch) aNOEXCEPT
		{
			mData.insert(pos, ch);
		}
		iterator erase(iterator pos) aNOEXCEPT
		{
			return mData.erase(pos);
		}
		iterator erase(const_iterator pos) aNOEXCEPT
		{
			return mData.erase(pos);
		}
		iterator erase(iterator bg, iterator ed) aNOEXCEPT
		{
			return mData.erase(bg, ed);
		}
		iterator erase(const_iterator bg, const_iterator ed) aNOEXCEPT
		{
			return mData.erase(bg, ed);
		}
		iterator find(Type ch) aNOEXCEPT
		{
			return mData.find(ch);
		}
		const_iterator find(Type ch) const aNOEXCEPT
		{
			return mData.find(ch);
		}
		const_iterator find(const self_type& str) const aNOEXCEPT
		{
			if (mSize < str.mSize)
				return end();
			for (size_type cur = 0; cur < mSize; ++cur)
			{
				if (cur + str.mSize > mSize)
					break;
				bool found = false;
				for (size_type i = 0; i < str.mSize; ++i)
				{
					if (mData[cur + i] != str.mData[i])
					{
						found = true;
						break;
					}
				}
				if (!found)
					return mData + cur;
			}
			return end();
		}
		iterator rfind(Type ch) aNOEXCEPT
		{
			return mData.rfind(ch);
		}
		const_iterator rfind(Type ch) const aNOEXCEPT
		{
			return mData.rfind(ch);
		}
		//find a string from the back
		const_iterator rfind(const self_type& str) const aNOEXCEPT
		{
			for (long cur = mSize - str.mSize; cur >= 0; --cur)
			{
				bool flag = false;
				for (size_type i = 0; i < str.mSize; ++i)
				{
					if (mData[cur + i] != str[i])
					{
						flag = true;
						break;
					}
				}
				if (!flag)
					return mData + str.mSize;
			}
			return end();
		}
		bool contain(Type ch)
		{
			return find(ch) != end();
		}
		bool contain(const self_type& str)
		{
			return find(str) != end();
		}

		bool replace(Type oldVal, Type newVal) aNOEXCEPT
		{
			bool flag = false;
			for (size_type cur = 0; cur < mSize; ++cur)
			{
				if (mData[cur] == oldVal)
				{
					flag = true;
					mData[cur] = newVal;
				}
			}
			return flag;
		}
		bool replaceFirst(Type oldVal, Type newVal) aNOEXCEPT
		{
			for (size_type cur = 0; cur < mSize; ++cur)
			{
				if (mData[cur] == oldVal)
				{
					mData[cur] = newVal;
					return true;
				}
			}
			return false;
		}
		bool replaceLast(Type oldVal, Type newVal) aNOEXCEPT
		{
			for (long cur = mSize - 1; cur >= 0; cur--)
			{
				if (mData[cur] == oldVal)
				{
					mData[cur] = newVal;
					return true;
				}
			}
			return false;
		}
		bool replaceIndex(size_type index, Type oldVal, Type newVal) aNOEXCEPT;

		bool replace(self_type&& oldVal, self_type&& newVal) aNOEXCEPT;
		bool replaceFirst(self_type&& oldVal, self_type&& newVal) aNOEXCEPT;
		bool replaceLast(self_type&& oldVal, self_type&& newVal) aNOEXCEPT;
		bool replaceIndex(size_type index, self_type&& oldVal, self_type&& newVal) aNOEXCEPT;
		iterator begin() aNOEXCEPT
		{
			return mData.begin();
		}
		const_iterator begin() const aNOEXCEPT
		{
			return mData.begin();
		}
		const_iterator cbegin() const aNOEXCEPT
		{
			return mData.cbegin();
		}
		iterator end() aNOEXCEPT
		{
			return mData.end();
		}
		const_iterator end() const aNOEXCEPT
		{
			return mData.end();
		}
		const_iterator cend() const aNOEXCEPT
		{
			return mData.cend();
		}
		self_type& operator = (const self_type& other) aNOEXCEPT
		{
			*this = std::move(other);
		}
		self_type& operator = (self_type&& other) aNOEXCEPT
		{
			resize(other.mSize);
			for (size_type cur = 0; cur < other.mSize; ++cur)
			{
				mData[cur] = other.mData[cur];
			}
			return *this;
		}
		self_type& operator += (Type ch) aNOEXCEPT
		{
			push_back(ch);
			return *this;
		}
		self_type& operator += (const self_type& other) aNOEXCEPT
		{
			for (size_type i = 0; i < other.mSize; i++)
			{
				push_back(other[i]);
			}
			return *this;
		}
		
		bool operator < (const self_type& other) const aNOEXCEPT
		{
			return mData < other.mData;
		}
		bool operator >(const self_type& other) const aNOEXCEPT
		{
			return mData > other.mData;
		}
		bool operator == (const self_type& other) const aNOEXCEPT
		{
			return mData == other.mData;
		}
		bool operator != (const self_type& other) const aNOEXCEPT
		{
			return mData != other.mData;
		}
		bool operator <= (const self_type& other) const aNOEXCEPT
		{
			return mData <= other.mData;
		}
		bool operator >= (const self_type& other) const aNOEXCEPT
		{
			return mData >= other.mData;
		}
		bool operator [] (size_type index) const aNOEXCEPT
		{
			return mData[index];
		}
	private:
		aVector<Type,Allocator> mData;
		size_type mSize;
		size_type mCapacity;
		static_assert(aIsCharLike<Type>::value,
			"The type of element is not char-like objects.");
	};

	typedef aString_t<char> aString;
}
