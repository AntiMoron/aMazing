#include<string>
#include<assert.h>
#include<fstream>
#include<iostream>
using namespace std;

std::string openFile(const char* p)
{
	std::ifstream fStream(p);
	if (!fStream.is_open())
	{
		assert("File Not Open");
	}

	return std::string(std::istreambuf_iterator<char>(fStream),
		std::istreambuf_iterator<char>());
}

int main(int argV,char* args[])
{
	if(argV != 3)
	{
		cout << "arguments not correct." << endl;
		return -1;
	}
	if(std::string(args[1]) == std::string(args[2]))
	{
		cout << "Desc filepath can't be the same as Src filepath!!!" << endl;
		return -1;
	}
	std::string content = openFile(args[1]);
	std::string result = "\"";
	for(size_t i=0; i < content.size();i++)
	{
		if(content[i] == '\"' || content[i] == '\'')
		{
			result += '\\';
		}
		else if(content[i] == '\n')
		{
			result += '\"';
			result += '\n';
			result += '\"';
		}
		else result += content[i];
	}
	if(result.back() != '\"')
		result += '\"';
	else if(result.size() >= 2 && result.back() == result[result.size() - 1] && result.back() == '\"')
		result.pop_back();
	std::ofstream oStream(args[2]);
	oStream.write(result.c_str(),result.size());
	return 0;
}
