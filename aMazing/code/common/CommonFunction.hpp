#pragma once
#include"CommonDxSupport.hpp"
#include<cmath>
#include<string>

namespace aMazing
{
	extern bool isBlank(char ch);
	//将值val按比例增长
	extern float increaseByScale(float val, float scale, float upperBound, float bias);
	//将值val按比例衰减
	extern float fadeByScale(float val, float scale, float lowerBound, float bias);

	//将值val按照数值减少
	extern float fadeByValue(float val, float decreasement, float lowerBound);

	//将值val按数值增长
	extern float increaseByValue(float val, float increasement, float upperBound);
	//将三维向量单位化
	extern XMFLOAT3 normalize(XMFLOAT3 src);
	//get the hash code of c-style string literal
	extern unsigned long long hashCStringLiteral(const char* lit);
}
