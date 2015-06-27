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

		/*
		ABC所在圆与D的位置判断
		返回值大于0 则D位于圆的内部
		小于0 则位于圆的外部
		等于0 则四点共圆
		*/
		template<typename Type>
		Type aInCircle(const aVec2_t<Type>& a,
			const aVec2_t<Type>& b, 
			const aVec2_t<Type>& c, 
			const aVec2_t<Type>& d)
		{
			return makeMatrix(a.x - d.x, a.y - d.y, aSQUARE(a.x - d.x) + aSQUARE(a.y - d.y),
				b.x - d.x, b.y - d.y, aSQUARE(b.x - d.x) + aSQUARE(b.y - d.y),
				c.x - d.x, c.y - d.y, aSQUARE(c.x - d.x) + aSQUARE(c.y - d.y)).determinant();
		}
	}
}
