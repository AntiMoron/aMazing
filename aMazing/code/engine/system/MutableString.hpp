#pragma once
#include<string>
#include<codecvt>
#include<locale>
namespace aMazing
{
	class MutableString
	{
	public:
		MutableString();
		~MutableString();

		MutableString(const MutableString& other);
		MutableString(const std::string& other);
		MutableString(std::string&& other);
		MutableString(const std::wstring& other);
		MutableString(std::wstring&& other);
		MutableString(const char* other);
		MutableString(const wchar_t* other);
	
		void clear();

		MutableString& operator = (const MutableString& other);
		MutableString& operator += (const wchar_t ch);
		MutableString& operator += (const char ch);

		std::string getMultiByteString();
		std::wstring getWideString();
	private:
		bool strUpdated;
		bool wstrUpdated;
		std::string str;
		std::wstring wstr;
	};
}
