#pragma once

#include<cstdio>
#include<string>
#include<D3D11.h>
#include<D3DX11.h>
#include"MutableString.hpp"
namespace aMazing
{
	class FileTracker
	{
	public:
		FileTracker();
		~FileTracker();

		MutableString& getContext();

		bool LoadFile(MutableString& filename);
		bool LoadFile(MutableString&& filename);
		bool LoadBinaryFile(MutableString& filename);
		bool LoadBinaryFile(MutableString&& filename);

		bool WriteFile(MutableString& filename);
		bool WriteFile(MutableString&& filename);
		bool WriteBinaryFile(MutableString& filename);
		bool WriteBinaryFile(MutableString&& filename);
	private:
		FILE* fp;
		MutableString context;
	};
}
