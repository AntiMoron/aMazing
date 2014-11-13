#include"CommonUtil.h"



float increaseByScale(float val, float scale, float upperBound,float bias)
{
	if (val > upperBound - bias)
	{
		return upperBound;
	}
	return val * scale;
}

float fadeByScale(float val, float scale, float lowerBound,float bias)
{
	if (val > 0 && val < lowerBound + bias)
	{
		return lowerBound;
	}
	if (val < 0 && val > lowerBound - bias)
	{
		return lowerBound;
	}
	return val * scale;
}

float fadeByValue(float val, float decreasement, float lowerBound)
{
	val -= decreasement;
	if (val < lowerBound)
	{
		return lowerBound;
	}
	return val;
}

float increaseByValue(float val,float increasement,float upperBound)
{
	val += increasement;
	if (val > upperBound)
	{
		return upperBound;
	}
	return val;
}


XMFLOAT3 normalize(XMFLOAT3 src)
{
	float length = src.x * src.x;
	length += src.y * src.y;
	length += src.z * src.z;
	length = sqrt(length);
	if (length == 0.0f)
		return src;
	src.x /= length;
	src.y /= length;
	src.z /= length;
	return src;
}

bool aiStringLess::operator()(const aiString& aiA, const aiString& aiB)const
{
	std::string a = aiA.C_Str();
	std::string b = aiB.C_Str();
	return a < b;
}