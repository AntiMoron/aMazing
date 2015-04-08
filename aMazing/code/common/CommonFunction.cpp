#include"CommonFunction.hpp"

bool isBlank(char ch)
{
	return ch == ' ' ||
		ch == '\t' ||
		ch == '\r' ||
		ch == '\n';
}

//将值val按比例增长
float aMazing::increaseByScale(float val, float scale, float upperBound, float bias)
{
	if (val > upperBound - bias)
	{
		return upperBound;
	}
	return val * scale;
}

//将值val按比例衰减
float aMazing::fadeByScale(float val, float scale, float lowerBound, float bias)
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

//将值val按照数值减少
float aMazing::fadeByValue(float val, float decreasement, float lowerBound)
{
	val -= decreasement;
	if (val < lowerBound)
	{
		return lowerBound;
	}
	return val;
}

//将值val按数值增长
float aMazing::increaseByValue(float val, float increasement, float upperBound)
{
	val += increasement;
	if (val > upperBound)
	{
		return upperBound;
	}
	return val;
}

//将三维向量单位化
XMFLOAT3 aMazing::normalize(XMFLOAT3 src)
{
	float length = src.x * src.x;
	length += src.y * src.y;
	length += src.z * src.z;
	length = std::sqrt(length);
	if (length == 0.0f)
		return src;
	src.x /= length;
	src.y /= length;
	src.z /= length;
	return src;
}
