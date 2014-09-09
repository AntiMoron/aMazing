#include "MutableString.h"


MutableString::MutableString()
{
	str = "";
	wstr = L"";
	strUpdated = false;
	wstrUpdated = false;
}


MutableString::~MutableString()
{
	;
}
MutableString::MutableString(const MutableString& other)
{
	strUpdated = other.strUpdated;
	wstrUpdated = other.wstrUpdated;
	str = other.str;
	wstr = other.wstr;
}

MutableString::MutableString(const std::string& other)
{
	strUpdated = true; 
	wstrUpdated = false;
	str = other;
}
MutableString::MutableString(std::string&& other)
{
	strUpdated = true;
	wstrUpdated = false;
	str = other;
}
MutableString::MutableString(const std::wstring& other)
{
	strUpdated = false;
	wstrUpdated = true;
	wstr = other;
}
MutableString::MutableString(std::wstring&& other)
{
	strUpdated = false;
	wstrUpdated = true;
	wstr = other;
}
MutableString::MutableString(const char* other)
{
	strUpdated = true;
	wstrUpdated = false;
	str = other;
}
MutableString::MutableString(const wchar_t* other)
{
	strUpdated = false;
	wstrUpdated = true;
	wstr = other;
}

void MutableString::clear()
{
	strUpdated = true;
	wstrUpdated = true;
	wstr.clear();
	str.clear();
}

MutableString& MutableString::operator = (const MutableString& other)
{
	str = other.str;
	wstr = other.wstr;
	strUpdated = other.strUpdated;
	wstrUpdated = other.wstrUpdated;
	return *this;
}

MutableString& MutableString::operator += (const wchar_t ch)
{
	if (strUpdated == true)
	{
		if (wstrUpdated == false)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			wstr = converter.from_bytes(str.c_str());
		}
		wstrUpdated = true;
		wstr += ch;
		strUpdated = false;
	}
	else
	{
		if (wstrUpdated == true)
		{
			wstr += ch;
			strUpdated = false;
		}
	}
	return *this;
}

MutableString& MutableString::operator += (const char ch)
{
	if (wstrUpdated == true)
	{
		if(strUpdated == false)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			str = converter.to_bytes(wstr.c_str());
		}
		strUpdated = true;
		str += ch;
		wstrUpdated = false;
	}
	else
	{
		if (strUpdated == true)
		{
			str += ch;
			wstrUpdated = false;
		}
	}
	return *this;
}

std::string MutableString::getMultiByteString()
{
	if (strUpdated == false && wstrUpdated == true)
	{
		strUpdated = true;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		str = converter.to_bytes(wstr.c_str());
		return str;
	}
	else
	{
		if (strUpdated == false)
			return "";
	}
	return str;
}

std::wstring MutableString::getWideString()
{
	if (strUpdated == true && wstrUpdated == false)
	{
		wstrUpdated = true;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		wstr = converter.from_bytes(str.c_str());
		return wstr;
	}
	else
	{
		if (wstrUpdated == false)
		{
			return L"";
		}
	}
	return wstr;
}