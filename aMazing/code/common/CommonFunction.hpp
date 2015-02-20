#pragma once
#include"CommonDxSupport.hpp"
#include<cmath>
#include<string>

namespace aMazing{
	//将值val按比例增长
	float increaseByScale(float val, float scale, float upperBound, float bias);
	//将值val按比例衰减
	float fadeByScale(float val, float scale, float lowerBound, float bias);

	//将值val按照数值减少
	float fadeByValue(float val, float decreasement, float lowerBound);

	//将值val按数值增长
	float increaseByValue(float val, float increasement, float upperBound);
	//将三维向量单位化
	XMFLOAT3 normalize(XMFLOAT3 src);
}