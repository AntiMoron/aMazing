#pragma once
#include<memory>
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
		typedef aMemory<Allocator> aMem;
		const static size_t DEFAULT_SIZE = 11;
		const static size_t VECTOR_INCREMENT = 20;
		T* createMemory(size_t count)
		{
			return aMem::createMemory<T>(get_allocator(),count);
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
		typedef aRandomAccessIterator<T> iterator;
		typedef aRandomAccessIterator<const T> const_iterator;
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
			auto& allocator = get_allocator();
			deleteMemory(mData);
		}

		static allocator_type& get_allocator()
		{
			static allocator_type alloc;
			return alloc;
		}

		T& operator[] (size_type index)
		{
			return mData[index];
		}

		T* data() const aNOEXCEPT
		{
			return mData;
		}
		void swap(aVector<T>& other) aNOEXCEPT
		{
			aVector::swap(std::forward<aVector<T> >(other));
		}
		void swap(aVector<T>&& other) aNOEXCEPT
		{
			aSwap(mData, other.mData);
			aSwap(mSize, other.mSize);
			aSwap(mCapacity, other.mCapacity);
		}

		void push_back(T& val) aNOEXCEPT
		{
			push_back(std::forward<T>(std::move(val)));
		}

		void push_back(T&& val) aNOEXCEPT
		{
			size_type newIndex = mSize;
			if (newIndex >= mCapacity)
			{
				expandSpace();
			}
			mData[newIndex] = val;
			mSize = newIndex + 1;
		}
		void pop_back() aNOEXCEPT
		{
			if (std::is_destructible<T>::value)
			{
				std::allocator_traits<Allocator>::destroy(mData + mSize - 1);
			}
			--mSize;
		}

		size_type size() const aNOEXCEPT
		{
			return mSize;
		}

		size_type capacity() const aNOEXCEPT
		{
			return mCapacity;
		}

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

		void shrink() aNOEXCEPT
		{
			auto& allocator = get_allocator();
			T* newPtr = std::allocator_traits<A>::allocate(allocator, mSize);
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

		void insert(iterator pos, T& key)
		{
			insert(pos, std::forward<T>(std::move(key)));
		}
		void insert(iterator pos, T&& key)
		{
			push_back(key);
			for (auto it = end() - 1; it != pos; --it)
			{
				*it = *(it - 1);
			}
			*pos = key;
		}

		void insert(const_iterator pos, T& key)
		{
			insert(pos, std::forward<T>(std::move(key)));
		}
		
		void insert(const_iterator pos, T&& key)
		{
			insert(iterator(pos),key);
		}

		iterator erase(iterator pos)
		{
			auto& allocator = get_allocator();
			size_t distance = pos - begin();
			for (size_t cur = 0; cur < distance; cur++)
			{
				std::allocator_traits<Allocator>::destroy(allcator, mData + distance);
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

		iterator erase(const_iterator pos)
		{
			return erase(iterator(pos));
		}

		//erase elements in rangeg [bg,ed)
		iterator erase(iterator bg,iterator ed)
		{
			auto& allocator = get_allocator();
			size_t distance = bg - begin();
			for (auto it = ed; it != end(); it++)
			{
				size_t dist = it - begin();
				*(mData + dist) = *it;
				std::allocator_traits<Allocator>::destroy(allocator, mData + (it - begin()));
			}
			mSize -= (ed - bg);
			return bg;
		}

		iterator erase(const_iterator bg, const_iterator ed)
		{
			return erase(iterator(bg), iterator(ed));
		}

		const_iterator begin() const aNOEXCEPT
		{
			return const_iterator(mData);
		}
		const_iterator end() const aNOEXCEPT
		{
			return const_iterator(mData + mSize);
		}
		const_iterator cbegin() const aNOEXCEPT
		{
			return const_iterator(mData);
		}
		const iterator cend() const aNOEXCEPT
		{
			return const_iterator(mData + mSize);
		}
	private:
		void expandSpace()
		{
			auto& allocator = get_allocator();
			size_type newCapacity = mCapacity + VECTOR_INCREMENT;
			T* newPtr = createMemory(newCapacity);
			for (size_type i = 0; i < mCapacity; i++)
			{
				newPtr[i] = mData[i];
			}
			if (!!mData)
			{
				deleteMemory(mData);
				mData = newPtr;
			}
			mCapacity = newCapacity;
		}
		size_type mSize;
		size_type mCapacity;
		T* mData;
		static_assert(aIsInsertable<aVector<T, Allocator>, Allocator, T>::value,
			"Element type is neither CopyInsertable nor MoveInsertable");
	};
}
