#pragma once
#include<memory>
#include<iterator>
#include"../../common/CommonTemplates.hpp"
#include"../system/memory/Memory.hpp"
#include"./CommonContainer.hpp"
#include"./Iterator.hpp"

namespace aMazing
{
	template<typename T,typename Allocator = std::allocator<T> >
	class aVector : public CommonContainer
	{
	private:
		typedef aVector<T, Allocator> self_type;
		typedef typename std::remove_const<
			typename std::remove_reference<T>::type>::type rawType;
		typedef aMemory<Allocator> aMem;
		const static size_t DEFAULT_SIZE = 100;
		const static size_t VECTOR_INCREMENT = 200;
		T* createMemory(size_t count)
		{
			return aMem::createMemory<T>(get_allocator(), count);
		}
		void deleteMemory(T* p)
		{
			aMem::deleteMemory<T>(get_allocator(), p);
		}
		void basicInitialize() aNOEXCEPT
		{
			mSize = 0;
			mCapacity = DEFAULT_SIZE;
			mData = createMemory(mCapacity);
		}
	public:
		typedef Allocator allocator_type;
		typedef size_t size_type;
		typedef aRandomAccessIterator<rawType> iterator;
		typedef aRandomAccessIterator<const rawType> const_iterator;
		explicit aVector() aNOEXCEPT
		{
			basicInitialize();
		}

		aVector(const aVector<T>& other) aNOEXCEPT
		{
			basicInitialize();
			resize(other.size());
			for (size_type cur = 0; cur < other.size(); cur++)
			{
				mData[cur] = other.mData[cur];
			}
		}

		aVector(aVector<T>&& other) aNOEXCEPT
		{
			basicInitialize();
			this->swap(other);
		}

		~aVector()
		{
			deleteMemory(mData);
		}

		static allocator_type& get_allocator()
		{
			static allocator_type alloc;
			return alloc;
		}
		
		void clear() aNOEXCEPT
		{
			auto& allocator = get_allocator();
			if (std::is_destructible<T>::value)
			{
				for (size_t cur = 0; cur < mSize; ++cur)
				{
					std::allocator_traits<Allocator>::destroy(allocator, mData + cur);
				}
			}
			mSize = 0;
		}

		bool empty() const aNOEXCEPT
		{
			return mSize == 0;
		}

		T* data() const aNOEXCEPT
		{
			return mData;
		}
		void swap(aVector<T>& other) aNOEXCEPT
		{
			aSwap(mData, other.mData);
			aSwap(mSize, other.mSize);
			aSwap(mCapacity, other.mCapacity);
		}
		
		void swap(aVector<T>&& other) aNOEXCEPT
		{
			aSwap(mData, other.mData);
			aSwap(mSize, other.mSize);
			aSwap(mCapacity, other.mCapacity);
		}

		iterator find(rawType& e) aNOEXCEPT
		{
			return find(static_cast<rawType&&>(e));
		}
		iterator find(rawType&& e) aNOEXCEPT
		{
			for (size_type cur = 0; cur < mSize; ++cur)
			{
				if (mData[cur] == e)
				{
					return mData + cur;
				}
			}
			return end();
		}

		const_iterator find(rawType& e) const aNOEXCEPT
		{
			return find(static_cast<rawType&&>(e));
		}

		const_iterator find(rawType&& e) const aNOEXCEPT
		{
			for (size_type cur = 0; cur < mSize; ++cur)
			{
				if (mData[cur] == e)
				{
					return mData + cur;
				}
			}
			return end();
		}

		iterator findByIndex(size_type index, rawType& e) aNOEXCEPT
		{
			return findByIndex(index, static_cast<rawType&&>(e));
		}

		iterator findByIndex(size_type index, rawType&& e) aNOEXCEPT
		{
			size_type valCnt = 0;
			for (size_type cur = 0; cur < mSize; ++cur)
			{
				if (mData[cur] == e)
				{
					if (valCnt == index)
					{
						return mData + cur;
					}
					++valCnt;
				}
			}
			return end();
		}

		const_iterator findByIndex(size_type index, rawType& e) const aNOEXCEPT
		{
			return findByIndex(index, static_cast<rawType&&>(e));
		}

		const_iterator findByIndex(size_type index, rawType&& e) const aNOEXCEPT
		{
			size_type valCnt = 0;
			for (size_type cur = 0; cur < mSize; ++cur)
			{
				if (mData[cur] == e)
				{
					if (valCnt == index)
					{
						return mData + cur;
					}
					++valCnt;
				}
			}
			return end();
		}

		iterator rfind(rawType& e) aNOEXCEPT
		{
			return rfind(std::move(e));
		}
		
		iterator rfind(rawType&& e) aNOEXCEPT
		{
			for (long cur = mSize - 1; cur >= 0; --cur)
			{
				if (mData[cur] == e)
				{
					return mData + cur;
				}
			}
			return end();
		}
		//find the element in a reversed ordered.
		//If the element is not found then return end()
		const_iterator rfind(rawType& e) const aNOEXCEPT
		{
			return rfind(static_cast<rawType&&>(e));
		}
		//find the element in a reversed ordered.
		//If the element is not found then return end()
		const_iterator rfind(rawType&& e) const aNOEXCEPT
		{
			for (long cur = mSize - 1; cur >= 0; --cur)
			{
				if (mData[cur] == e)
				{
					return mData + cur;
				}
			}
			return end();
		}
		//Append an element into the vector at the end.
		void push_back(const rawType& val) aNOEXCEPT
		{
			push_back(static_cast<rawType&&>(const_cast<rawType&>(val)));
		}

		//Append an element into the vector at the end.
		void push_back(rawType&& val) aNOEXCEPT
		{
			size_type newIndex = mSize;
			if (newIndex >= mCapacity)
			{
				expandSpace();
			}
			mData[newIndex] = val;
			mSize = newIndex + 1;
		}
		//erase the last element.
		void pop_back() aNOEXCEPT
		{
			if (std::is_destructible<T>::value)
			{
				std::allocator_traits<Allocator>::destroy(get_allocator(),mData + mSize - 1);
			}
			--mSize;
		}
		//return the size of the vector.
		size_type size() const aNOEXCEPT
		{
			return mSize;
		}
		//return the capacity of the vector.
		size_type capacity() const aNOEXCEPT
		{
			return mCapacity;
		}
		//resize the size of the vector.
		void resize(size_type newSize)
		{
			if (newSize < mCapacity)
			{
				mSize = newSize;
			}
			else
			{
				T* newPtr = createMemory(newSize);
				aSwap(mData, newPtr);
				if (!!newPtr)
				{
					deleteMemory(newPtr);
					newPtr = nullptr;
				}
				mCapacity = newSize;
				mSize = newSize;
			}
		}
		//shrink the capacity to fit the size.
		void shrink() aNOEXCEPT
		{
			auto& allocator = get_allocator();
			T* newPtr = createMemory(mSize);
			for (size_type cur = 0; cur < mSize; cur++)
			{
				newPtr[cur] = mData[cur];
			}
			if (!!mData)
			{
				deleteMemory(mData);
				mData = newPtr;
			}
			mCapacity = mSize;
		}
		//Insert an element into given position.
		void insert(iterator pos, T& key)
		{
			insert(pos, static_cast<T&&>(key));
		}
		//Insert an element into given position.
		void insert(iterator pos, T&& key)
		{
			push_back(key);
			for (auto it = end() - 1; it != pos; --it)
			{
				*it = *(it - 1);
			}
			*pos = key;
		}
		//Insert an element into given position.
		void insert(const_iterator pos, T& key)
		{
			push_back(key);
			for (auto it = end() - 1; it != pos; --it)
			{
				*it = *(it - 1);
			}
			*(begin() + (pos - cbegin()));
		}
		//Erase an element at given position.
		iterator erase(iterator pos)
		{
			auto& allocator = get_allocator();
			size_t distance = pos - begin();
			if (std::is_destructible<T>::value)
			{
				std::allocator_traits<Allocator>::destroy(allocator, mData + distance);
			}
			iterator endPos = end() - 1;
			for (auto it = pos; it != endPos; ++it)
			{
				*it = *(it + 1);
			}
			--mSize;
			//return iterator of next element.
			if (pos == end())
			{
				return iterator(mData + mSize);
			}
			return pos;
		}
		//Erase elements in range [bg,ed).
		iterator erase(iterator bg,iterator ed)
		{
			auto& allocator = get_allocator();
			iterator returnVal = bg;
			for (; bg != end() && ed != end(); ++bg, ++ed)
			{
				*bg = *ed;
				if (std::is_destructible<T>::value)
				{
					std::allocator_traits<Allocator>::destroy(allocator, mData + (ed - begin()));
				}
			}
			mSize -= (ed - bg);
			return returnVal;
		}
		iterator begin() aNOEXCEPT
		{
			return mData;
		}
		const_iterator begin() const aNOEXCEPT
		{
			return mData;
		}
		const_iterator cbegin() const aNOEXCEPT
		{
			return mData;
		}
		iterator end() aNOEXCEPT
		{
			return mData + mSize;
		}
		const_iterator end() const aNOEXCEPT
		{
			return mData + mSize;
		}
		const_iterator cend() const aNOEXCEPT
		{
			return mData + mSize;
		}

		T& front() const aNOEXCEPT
		{
			return *mData;
		}

		T& back() const aNOEXCEPT
		{
			return *(mData + mSize - 1);
		}
		T& operator[] (size_type index) const aNOEXCEPT
		{
			return mData[index];
		}

		bool operator != (const self_type& other) const aNOEXCEPT
		{
			if (mSize != other.mSize)
				return true;
			for (size_type i = 0; i < mSize; i++)
			{
				if (mData[i] != other.mData[i])
					return true;
			}
			return false;
		}
		bool operator == (const self_type& other) const aNOEXCEPT
		{
			if (mSize != other.mSize)
				return false;
			for (size_type i = 0; i < mSize; i++)
			{
				if (mData[i] != other.mData[i])
					return false;
			}
			return true;
		}

		bool operator < (const self_type& other) const aNOEXCEPT
		{
			size_type cur1 = 0, cur2 = 0;
			for (;cur1 != mSize && cur2 != other.mSize;)
			{
				if (mData[cur1] == other.mData[cur2])
				{
					if (cur1 < mSize)
					{
						++cur1;
					}
					if (cur2 < other.mSize)
					{
						++cur2;
					}
					continue;
				}
				else
				{
					return mData[cur1] < other.mData[cur2];
				}
			}
			return mSize < other.mSize;
		}

		bool operator > (const self_type& other) const aNOEXCEPT
		{
			size_type cur1 = 0, cur2 = 0;
			for (; cur1 != mSize && cur2 != other.mSize;)
			{
				if (mData[cur1] == other.mData[cur2])
				{
					if (cur1 < mSize)
					{
						++cur1;
					}
					if (cur2 < other.mSize)
					{
						++cur2;
					}
					continue;
				}
				else
				{
					return mData[cur1] > other.mData[cur2];
				}
			}
			return mSize > other.mSize;
		}
		bool operator <= (const self_type& other) const aNOEXCEPT
		{
			return !((*this) > other);
		}
		bool operator >= (const self_type& other) const aNOEXCEPT
		{
			return !((*this) < other);
		}
	private:
		void expandSpace()
		{
			size_type newCapacity = mCapacity + VECTOR_INCREMENT;
			T* newPtr = createMemory(newCapacity);
			for (size_type i = 0; i < mCapacity; i++)
			{
				newPtr[i] = mData[i];
			}
			if (!!mData)
			{
				deleteMemory(mData);
			}
			mData = newPtr;
			mCapacity = newCapacity;
		}
		size_type mSize;
		size_type mCapacity;
		T* mData;
		static_assert(aIsInsertable<aVector<T, Allocator>, Allocator, T>::value,
			"Element type is neither CopyInsertable nor MoveInsertable");
	};
}
