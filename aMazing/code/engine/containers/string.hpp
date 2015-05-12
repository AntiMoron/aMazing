#pragma once
#include<memory>
#include"../../common/CommonDef.hpp"
#include"../exception/MemoryException.hpp"
#include"check/CharLike.hpp"
#include"./vector.hpp"

namespace aMazing
{
	template<typename Type,typename Allocator = std::allocator<Type> >
	class aString_t
	{
	public:
		typedef size_t size_type;
		typedef Allocator allocator_type;
		typedef typename aVector<Type, Allocator>::iterator iterator;
		typedef typename aVector<Type, Allocator>::const_iterator const_iterator;
	private:
		typedef aString_t<Type, Allocator> self_type;
		typedef typename std::remove_const<typename std::remove_reference<Type>::type>::type rawType;

		const_iterator findInRange(self_type&& str, size_type bg, size_type ed) const aNOEXCEPT
		{
			if ((ed - bg) < str.size())
			return end();
			for (size_type cur = bg; cur < ed; ++cur)
			{
				if (cur + str.size() > size())
					break;
				bool found = false;
				for (size_type i = 0; i < str.size(); ++i)
				{
					if (mData[cur + i] != str.mData[i] && ((cur + i) >= ed))
					{
						found = true;
						break;
					}
				}
				if (!found)
					return begin() + cur;
			}
			return end();
		}

		iterator findInRange(self_type&& str, size_type bg, size_type ed) aNOEXCEPT
		{
			if ((ed - bg) < str.size())
			return end();
			for (size_type cur = bg; cur < ed; ++cur)
			{
				if (cur + str.size() > size())
					break;
				bool found = false;
				for (size_type i = 0; i < str.size(); ++i)
				{
					if (mData[cur + i] != str.mData[i] || ((cur + i) >= ed))
					{
						found = true;
						break;
					}
				}
				if (!found)
					return begin() + cur;
			}
			return end();
		}
		static Type*& getStaticPtr() aNOEXCEPT
		{
			static Type* ptr = nullptr;
			return ptr;
		}
	public:
		aString_t(){}
		aString_t(const Type* arr)
		{
			size_type cur = 0;
			while (arr[cur] != 0)
			{
				mData.push_back(arr[cur]);
				cur++;
			}
		}
		~aString_t()
		{
			Type* p = getStaticPtr();
			if (p != nullptr)
			{
				delete[] p;
				p = nullptr;
			}
		}
		const Type* c_str() const aNOEXCEPT
		{
			Type* ret = getStaticPtr();
			if (ret != nullptr)
			{
				delete[] ret;
				ret = nullptr;
			}
			ret = new Type[mData.size() + 1];
			for (size_type cur = 0; cur < mData.size(); ++cur)
			{
				ret[cur] = mData[cur];
			}
			ret[mData.size()] = 0;
			return ret;
		}
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

		self_type subString(size_type start, size_type sizeCnt = -1) const aNOEXCEPT
		{
			self_type ret;
			size_type endPos = start + sizeCnt;
			if (sizeCnt == -1 ||
				endPos < start ||
				endPos > size())
			{
				endPos = size();
			}
			for (size_type cur = start; cur < endPos; ++cur)
			{
				ret += mData[cur];
			}
			return ret;
		}
		
		aMazing::aVector<self_type> splitString(aVector<Type> chs) const aNOEXCEPT
		{
			aMazing::aVector<self_type> ret;
			size_type last = 0;
			size_type cur = 0;
			auto checkIsIn = [&](char curCh)->bool
			{
				for (auto ch : chs)
				{
					if(ch == curCh)
						return true;
				}
				return false;
			};
			while (cur < size())
			{
				if (cur < size() &&
					last == cur &&
					checkIsIn(mData[cur]))
				{
					cur++;
				}
				for (; cur < size() && !checkIsIn(mData[cur]); cur++);
				self_type sub;
				if (checkIsIn(mData[last]))
					sub = subString(last + 1, cur - last - 1);
				else
					sub = subString(last, cur - last);
				if (!sub.empty())
					ret.push_back(sub);
				last = cur;
			}
			return ret;
		}
		
		aMazing::aVector<self_type> splitString(Type ch) const aNOEXCEPT
		{
			aMazing::aVector<self_type> ret;
			size_type last = 0;
			size_type cur = 0;
			while (cur < size())
			{
				if (cur < size() &&
					last == cur &&
					mData[cur] == ch)
				{
					cur++;
				}
				for (; cur < size() && (mData[cur] != ch); cur++);
				self_type sub;
				if (mData[last] == ch)
					sub = subString(last + 1, cur - last - 1);
				else
					sub = subString(last, cur - last);
				if (!sub.empty())
					ret.push_back(sub);
				last = cur;
			}
			return ret;
		}
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
		void insert(iterator pos, const self_type& str) aNOEXCEPT
		{
			for (long cur = str.length() - 1; cur >= 0; --cur)
			{
				mData.insert(pos, str[cur]);
			}
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
		//Find a character.
		iterator find(Type ch) aNOEXCEPT
		{
			return mData.find(ch);
		}
		const_iterator find(Type ch) const aNOEXCEPT
		{
			return mData.find(ch);
		}
		//Find a string.
		iterator find(const self_type& str) aNOEXCEPT
		{
			return findInRange(static_cast<self_type&&>(const_cast<self_type&>(str)), 0, size());
		}
		iterator find(self_type&& str) aNOEXCEPT
		{
			return findInRange(std::move(str), 0, size());
		}
		const_iterator find(const self_type& str) const aNOEXCEPT
		{
			return findInRange(static_cast<self_type&&>(const_cast<self_type&>(str)), 0, size());
		}
		const_iterator find(self_type&& str) const aNOEXCEPT
		{
			return findInRange(std::move(str), 0, size());
		}
		//Find a character in reversed order.
		iterator rfind(Type ch) aNOEXCEPT
		{
			return mData.rfind(ch);
		}
		const_iterator rfind(Type ch) const aNOEXCEPT
		{
			return mData.rfind(ch);
		}
		//Find a string in reversed order.
		iterator rfind(const self_type& str) aNOEXCEPT
		{
			return rfind(static_cast<self_type>(const_cast<self_type&>(str)));
		}
		iterator rfind(self_type&& str) aNOEXCEPT
		{
			for (long cur = size() - str.size(); cur >= 0; --cur)
			{
				bool flag = false;
				for (size_type i = 0; i < str.size(); ++i)
				{
					if (mData[cur + i] != str[i])
					{
						flag = true;
						break;
					}
				}
				if (!flag)
					return begin() + str.size();
			}
			return end();
		}
		const_iterator rfind(const self_type& str) const aNOEXCEPT
		{
			return rfind(static_cast<self_type>(const_cast<self_type&>(str)));
		}
		const_iterator rfind(self_type&& str) const aNOEXCEPT
		{
			for (long cur = size() - str.size(); cur >= 0; --cur)
			{
				bool flag = false;
				for (size_type i = 0; i < str.size(); ++i)
				{
					if (mData[cur + i] != str[i])
					{
						flag = true;
						break;
					}
				}
				if (!flag)
					return begin() + str.size();
			}
			return end();
		}
		//find a character by index
		iterator findByIndex(size_type index, Type ch) aNOEXCEPT
		{
			return mData.findByIndex(index, ch);
		}

		const_iterator findByIndex(size_type index, Type ch) const aNOEXCEPT
		{
			return mData.findByIndex(index, ch);
		}
		//find a string by index
		iterator findByIndex(size_type index, const self_type& str) aNOEXCEPT
		{
			return findByIndex(index, static_cast<self_type&&>(const_cast<self_type&>(str)));
		}
	
		iterator findByIndex(size_type index, self_type&& str) aNOEXCEPT
		{
			size_type strSize = str.size();
			size_type limit = size() - strSize;
			size_type valCount = 0;
			iterator pos;
			for (size_type cur = 0; cur < limit; ++cur)
			{
				pos = findInRange(std::forward<self_type>(str), cur, cur + strSize);
				if (pos != end())
				{
					if (valCount == index)
					{
						return pos;
					}
					if (valCount > index)
						return end();
					++valCount;
				}
			}
			return end();
		}
		const_iterator findByIndex(size_type index, const self_type& str) const aNOEXCEPT
		{
			return findByIndex(index, static_cast<self_type&&>(const_cast<self_type&>(str)));
		}
		const_iterator findByIndex(size_type index, self_type&& str) const aNOEXCEPT
		{ 
			size_type strSize = str.size();
			size_type limit = size() - strSize;
			size_type valCount = 0;
			iterator pos;
			for (size_type cur = 0; cur < limit; ++cur)
			{
				pos = findInRange(std::forward<self_type>(str), cur, cur + strSize);
				if (pos != end())
				{
					if (valCount == index)
					{
						return pos;
					}
					if (valCount > index)
						return end();
					++valCount;
				}
			}
			return end();
		}

		bool contain(Type ch) const aNOEXCEPT
		{
			return find(ch) != end();
		}
		bool contain(const self_type& str) const aNOEXCEPT
		{
			return find(str) != end();
		}
		bool contain(self_type&& str) const aNOEXCEPT
		{
			return find(std::forward<self_type>(str)) != end();
		}

		bool replace(Type oldVal, Type newVal) aNOEXCEPT
		{
			bool flag = false;
			for (size_type cur = 0; cur < size(); ++cur)
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
			for (size_type cur = 0; cur < size(); ++cur)
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
			for (long cur = size() - 1; cur >= 0; cur--)
			{
				if (mData[cur] == oldVal)
				{
					mData[cur] = newVal;
					return true;
				}
			}
			return false;
		}
		bool replaceIndex(size_type index, Type oldVal, Type newVal) aNOEXCEPT
		{
			size_t valCount = 0;
			for (size_type cur = 0; cur < size(); ++cur)
			{
				if (mData[cur] == oldVal)
				{
					if (valCount == index)
					{
						mData[cur] = newVal;
						return true;
					}
					if (valCount > index)
						return false;
					++valCount;
				}
			}
			return false;
		}

		bool replaceIndex(size_type index, self_type&& oldVal, self_type&& newVal) aNOEXCEPT
		{
			size_type oldSize = oldVal.size();
			size_type limit = size() - oldSize;
			size_type valCount = 0;
			iterator pos;
			for (size_type cur = 0; cur < limit; ++cur)
			{
				pos = findInRange(std::forward<self_type>(oldVal), cur, cur + oldSize);
				if (pos != end())
				{
					if (valCount == index)
					{
						pos = erase(pos, pos + oldSize);
						insert(pos, newVal);
						return true;
					}
					++valCount;
				}
			}
			return false;
		}
		bool replace(self_type& oldVal, self_type& newVal) aNOEXCEPT
		{
			return replace(std::move(oldVal), std::move(newVal));
		}

		bool replace(self_type&& oldVal, self_type&& newVal) aNOEXCEPT
		{
			bool ret = false;
			iterator pos = findInRange(std::forward<self_type>(oldVal), 0, size());
			size_type oldSize = oldVal.size();
			while (pos != end())
			{
				ret = true;
				pos = erase(pos, pos + oldSize);
				insert(pos, newVal);
				pos = findInRange(std::forward<self_type>(oldVal), pos - begin() + newVal.size(), size());
			}
			return ret;
		}

		bool replaceFirst(self_type&& oldVal, self_type&& newVal) aNOEXCEPT
		{
			iterator pos = findInRange(std::forward<self_type>(oldVal), 0, size());
			size_type oldSize = oldVal.size();
			if (pos != end())
			{
				pos = erase(pos, pos + oldSize);
				insert(pos, newVal);
				return true;
			}
			return false;
		}
		bool replaceLast(self_type&& oldVal, self_type&& newVal) aNOEXCEPT
		{
			size_type oldSize = oldVal.size();
			iterator pos;
			for (int cur = size() - oldSize; cur >= 0; --cur)
			{
				pos = findInRange(std::forward<self_type>(oldVal), cur, cur + oldSize);
				if (pos != end())
				{
					pos = erase(pos, pos + oldSize);
					insert(pos, newVal);
					return true;
				}
			}
			return false;
		}
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
		self_type& operator = (self_type& other) aNOEXCEPT
		{
			*this = static_cast<self_type&&>(other);
			return *this;
		}
		self_type& operator = (self_type&& other) aNOEXCEPT
		{
			resize(other.size());
			for (size_type cur = 0; cur < other.size(); ++cur)
			{
				mData[cur] = other.mData[cur];
			}
			return *this;
		}
		self_type operator + (Type ch) const aNOEXCEPT
		{
			aString ret(*this);
			ret.push_back(ch);
			return 	ret;
		}
		self_type operator + (const self_type& other) const aNOEXCEPT
		{
			aString ret(*this);
			ret += other;
			return ret;
		}
		self_type& operator += (Type ch) aNOEXCEPT
		{
			push_back(ch);
			return *this;
		}
		self_type& operator += (const self_type& other) aNOEXCEPT
		{
			for (size_type i = 0; i < other.size(); i++)
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

		Type& operator [] (size_type index) const aNOEXCEPT
		{
			return mData[index];
		}

		friend std::ostream& operator << (std::ostream &o, const self_type &a)
		{
			for (size_type i = 0; i < a.size(); ++i)
			{
				o << a.mData[i];
			}
			return o;
		}
		struct hash
		{
			size_t operator() (const self_type& s) const aNOEXCEPT
			{
				size_t ret = 0;
				for (size_type cur = 0; cur < s.length(); ++cur)
				{
					ret *= 29;
					ret += s[cur];
				}
				return ret;
			}
		};
	private:
		aVector<Type,Allocator> mData;
		static_assert(aIsCharLike<Type>::value,
			"Element is not char-like objects.");
	};

	typedef aString_t<char> aString;
}
