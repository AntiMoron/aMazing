#pragma once
#include"../../engine/math/amath.hpp"


namespace aMazing
{
	namespace Math2D
	{
		/*
		返回值的绝对值是a,b,c三点的三角形面积的二倍
		如果大于0 三角形ABC为逆时针，C位于有向直线AB左侧
		如果小于0 三角形ABC为顺时针，C位于有向直线AB右侧
		如果等于0 则三点共线
		*/
		template<typename Type>
		Type aOrient2D(const aVec2_t<Type>& a,
			const aVec2_t<Type>& b,
			const aVec2_t<Type>& c)
		{
			return makeMatrix<Type>(a.x - c.x, a.y - c.y,
				b.x - c.x, b.y - c.y).determinant();
		}
	}
}
