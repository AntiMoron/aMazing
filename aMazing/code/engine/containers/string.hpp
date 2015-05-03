#pragma once
#include"../../common/CommonDef.hpp"
#include"../exception/MemoryException.hpp"
#include"check/CharLike.hpp"
#include"./vector.hpp"

namespace aMazing
{
	template<typename Type,typename Allocator = std::allocator<T> >
	class aString_t
	{
	private:
		typedef aString_t<Type,Allocator> self_type;
	public:
		typedef size_t size_type;
		typedef Allocator allocator_type;
		typedef typename aVector<Type,Allocator>::iterator iterator;
		typedef typename aVector<Type, Allocator>::const_iterator const_iterator;
		explicit aString(){}
		template<typename EType,size_t cnt>
		aString(const Type& p)
		{
			;
		}
		~aString(){}
		bool empty() const aNOEXCEPT;
		void clear() aNOEXCEPT;
		void resize() aNOEXCEPT;
		size_type size() const aNOEXCEPT;
		size_type length() const aNOEXCEPT;
		size_type capacity() const aNOEXCEPT;
		aMazing::aVector<self_type > splitString(char ch) const aNOEXCEPT;
		//aMazing::aVector<aString_t<Type> > splitString(Regex&&) const aNOEXCEPT;
		bool trim() aNOEXCEPT;
		void push_back() aNOEXCEPT;
		void pop_back() aNOEXCEPT;
		void insert(iterator pos,Type ch);
		void insert(const_iterator pos, Type ch);
		iterator erase(iterator pos);
		iterator erase(const_iterator pos);
		iterator erase(iterator bg,iterator ed);
		iterator erase(const_iterator bg, const_iterator ed);
		iterator find(Type ch);
		iterator find(const self_type& str);
		iterator reverseFind(Type ch);
		iterator reverseFind(const self_type& str);
		bool contain(Type ch);
		bool contain(const self_type& str);

		bool replace(Type oldVal,Type newVal) aNOEXCEPT;
		bool replaceFirst(Type oldVal, Type newVal) aNOEXCEPT;
		bool replaceLast(Type oldVal, Type newVal) aNOEXCEPT;
		bool replaceIndex(size_type index, Type oldVal, Type newVal) aNOEXCEPT;

		bool replace(self_type&& oldVal, self_type&& newVal) aNOEXCEPT;
		bool replaceFirst(self_type&& oldVal, self_type&& newVal) aNOEXCEPT;
		bool replaceLast(self_type&& oldVal, self_type&& newVal) aNOEXCEPT;
		bool replaceIndex(size_type index, self_type&& oldVal, self_type&& newVal) aNOEXCEPT;
		iterator begin() const aNOEXCEPT;
		const_iterator begin() const aNOEXCEPT;
		const_iterator cbegin() const aNOEXCEPT;
		iterator end() const aNOEXCEPT;
		const_iterator end() const aNOEXCEPT;
		const_iterator cend() const aNOEXCEPT;
		self_type& operator = (const self_type& other) aNOEXCEPT;
		self_type& operator = (self_type&& other) aNOEXCEPT;
		bool operator += (Type ch) aNOEXCEPT;
		bool operator += (const self_type& other) aNOEXCEPT;
		bool operator < (const self_type& other) const aNOEXCEPT;
		bool operator > (const self_type& other) const aNOEXCEPT;
		bool operator == (const self_type& other) const aNOEXCEPT;
		bool operator != (const self_type& other) const aNOEXCEPT;
		bool operator <= (const self_type& other) const aNOEXCEPT;
		bool operator >= (const self_type& other) const aNOEXCEPT;
		bool operator [] (size_type index) const throw(MemoryException);
	private:
		aVector<Type,Allocator> mData;
		size_type mSize;
		size_type mCapacity;
		static_assert(aIsCharLike<Type>::value,
			"The type of element is not char-like objects.");
	};

	typedef aString_t<char> aString;
}
