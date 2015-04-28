#pragma once
#include"../../common/CommonDef.hpp"
#include"./vector.hpp"

namespace aMazing
{
	template<typename T>
	class aString_t
	{
	public:
		typedef size_t size_type;
		aString();
		~aString(){}
		aString(const T* p);
		aString(const string&);
		bool empty() const aNOEXCEPT;
		void clear() aNOEXCEPT;
		size_type size() const aNOEXCEPT;
		size_type length() const aNOEXCEPT;
		size_type capacity() const aNOEXCEPT;
		aMazing::aVector<aString_t> splitString(char ch);
		;
	private:
		size_type mSize;
		size_type mCapacity;
		T* mData;
	};

	typedef aString_t<char> aString;
}
