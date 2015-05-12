#include"Directory.hpp"
using namespace aMazing;

aString aMazing::minimizeDirectory(const aString& dir)
{
	aString result;
	aVector<char> splashes;
	splashes.push_back('/');
	splashes.push_back('\\');
	auto splitedString = dir.splitString(splashes);
	size_t lastCur = -1;
	size_t nextCur = -1;
	for (size_t cur = 0; (cur + 1) < splitedString.size(); ++cur)
	{
		if (splitedString[cur] == ".")
		{
			continue;
		}
		else
		{
			lastCur = cur;
		}
		nextCur = cur + 1;
		while (nextCur < splitedString.size() &&
			splitedString[nextCur] == ".") ++nextCur;
		if (splitedString[nextCur] != "..")
		{
			result += splitedString[lastCur];
			result += '/';
		}
		else
		{
			if (splitedString[lastCur] == "..")
			{
				result += splitedString[lastCur];
				result += '/';
			}
			else
			{
				cur = nextCur + 1;
			}
		}
	}
	if (splitedString.back() != "..")
	{
		result += splitedString.back();
		if (!splitedString.back().contain('.'))
		{
			result += '/';
		}
	}
	return result;
}


aString aMazing::transformSplashes(const aString& dir)
{
	aString ret(dir);
	ret.replace('\\', '/');
	return ret;
}