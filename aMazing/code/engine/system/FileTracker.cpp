#include "FileTracker.h"


FileTracker::FileTracker()
{
	fp = nullptr;
}


FileTracker::~FileTracker()
{
	if (fp != nullptr)
	{
		fclose(fp);
	}
}

MutableString& FileTracker::getContext()
{
	return context;
}

bool FileTracker::LoadFile(MutableString& filename)
{
	fp = nullptr;
	std::string path = filename.getMultiByteString();
	fp = fopen(path.c_str(), "r");
	if (fp == nullptr)
	{
		printf("Open File Failed. File not found.\r\n");
		return false;
	}
	char ch;
	std::string text;
	while (!feof(fp))
	{
		ch = fgetc(fp);
		if (ch == EOF)
			break;
		text += ch;
	}
	context = text;
	if (fp != nullptr)
	{
		fclose(fp);
	}
	return true;
}

bool FileTracker::LoadFile(MutableString&& filename)
{
	fp = nullptr;
	std::string path = filename.getMultiByteString();
	fp = fopen(path.c_str(), "r");
	if (fp == nullptr)
	{
		return false;
	}
	char ch;
	std::string text;
	while (!feof(fp))
	{
		ch = fgetc(fp);
		if (ch == EOF)
			break;
		text += ch;
	}
	context = text;
	if (fp != nullptr)
	{
		fclose(fp);
	}
	return true;
}

bool FileTracker::LoadBinaryFile(MutableString& filename)
{
	fp = nullptr;
	std::string path = filename.getMultiByteString();
	fp = fopen(path.c_str(), "rb");
	if (fp == nullptr)
	{
		return false;
	}
	char ch;
	while (!feof(fp))
	{
		fread(&ch, 1, 1, fp);
		context += ch;
	}
	if (fp != nullptr)
	{
		fclose(fp);
	}
	return true;
}

bool FileTracker::LoadBinaryFile(MutableString&& filename)
{
	fp = nullptr;
	std::string path = filename.getMultiByteString();
	fp = fopen(path.c_str(), "rb");
	if (fp == nullptr)
	{
		return false;
	}	
	char ch;
	while (!feof(fp))
	{
		fread(&ch,1,1,fp);
		context += ch;
	}
	if (fp != nullptr)
	{
		fclose(fp);
	}
	return true;
}

bool FileTracker::WriteFile(MutableString& filename)
{
	std::string path = filename.getMultiByteString();

	fp = nullptr;
	fp = fopen(path.c_str(),"w");
	if (fp == nullptr)
	{
		return false;
	}

	std::string str = context.getMultiByteString();
	for (int i = 0; i < str.length(); i++)
	{
		fputc(str[i],fp);
	}

	if (fp != nullptr)
	{
		fclose(fp);
	}
	return true;
}
bool FileTracker::WriteFile(MutableString&& filename)
{
	std::string path = filename.getMultiByteString();

	fp = nullptr;
	fp = fopen(path.c_str(), "w");
	if (fp == nullptr)
	{
		return false;
	}

	std::string str = context.getMultiByteString();
	for (int i = 0; i < str.length(); i++)
	{
		fputc(str[i], fp);
	}

	if (fp != nullptr)
	{
		fclose(fp);
	}
	return true;
}

bool FileTracker::WriteBinaryFile(MutableString& filename)
{
	std::string path = filename.getMultiByteString();

	fp = nullptr;
	fp = fopen(path.c_str(), "wb");
	if (fp == nullptr)
	{
		return false;
	}

	std::string str = context.getMultiByteString();
	fwrite(str.c_str(), sizeof(char), str.length(), fp);

	if (fp != nullptr)
	{
		fclose(fp);
	}
	return true;
}
bool FileTracker::WriteBinaryFile(MutableString&& filename)
{
	std::string path = filename.getMultiByteString();

	fp = nullptr;
	fp = fopen(path.c_str(), "wb");
	if (fp == nullptr)
	{
		return false;
	}

	std::string str = context.getMultiByteString();
	fwrite(str.c_str(),sizeof(char),str.length(),fp);

	if (fp != nullptr)
	{
		fclose(fp);
	}
	return true;
}