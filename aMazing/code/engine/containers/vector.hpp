#pragma once
#include"../../common/CommonTemplates.hpp"
namespace aMazing
{
	#define DEFAULT_SIZE 11
	#define VECTOR_INCREMENT 20
	template<typename T>
	class aVector
	{
	private:
		void basicInitialize() aNOEXCEPT
		{
			mSize = 0;
			mCapacity = DEFAULT_SIZE;
			mData = new T[mCapacity];
		}
	public:
		typedef size_t size_type;
		explicit aVector() aNOEXCEPT
		{
			basicInitialize();
		}
		aVector(const aVector<T>& other) aNOEXCEPT
		{
			basicInitialize();
			resize(other.size());
			for (size_type cur = 0; cur<other.size(); cur++)
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
			if (std::is_destructible<T>::value)
			{
				for (size_type cur = 0; cur < mSize; cur++)
				{
					mData[cur].~T();
				}
			}
			if (!!mData)
			{
				delete[] mData;
				mData = nullptr;
			}
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
			cout << other.mSize << "|";
			aSwap(mData, other.mData);
			aSwap(mSize, other.mSize);
			aSwap(mCapacity, other.mCapacity);
			cout << mSize << "|";
		}

		void push_back(const T& val) aNOEXCEPT
		{
			push_back(const_cast<typename remove_reference<T>::type&&>(std::move(val)));
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
				mData[mSize - 1].~T();
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
				T* newPtr = new T[newSize];
				mCapacity = newSize;
				mSize = newSize;
				aSwap(mData, newPtr);
				if (!!newPtr)
				{
					delete[] newPtr;
					newPtr = nullptr;
				}
			}
		}

		void shrink() aNOEXCEPT
		{
			T* newPtr = new T[mSize];
			for (size_type cur = 0; cur < mSize; cur++)
			{
				newPtr[cur] = mData[cur];
			}
			if (!!mData)
			{
				delete[] newPtr;
				mData = newPtr;
			}
			mCapacity = mSize;
		}

		template<typename T>
		class iterator;
		
		typedef iterator<const T> const_iterator;
		const_iterator begin();
		const_iterator end();
		//check MoveInsertable && CopyInsertable.
	private:
		void expandSpace()
		{
			size_type newCapacity = mCapacity + VECTOR_INCREMENT;
			T* newPtr = new T[newCapacity];
			for (size_type i = 0; i < mCapacity; i++)
			{
				newPtr[i] = mData[i];
			}
			if (!!mData)
			{
				delete[] mData;
				mData = newPtr;
			}
			mCapacity = newCapacity;
		}
		size_type mSize;
		size_type mCapacity;
		T* mData;
	};
}