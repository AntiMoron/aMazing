#pragma once
#include"../../engine/math/amath.hpp"

namespace aMazing
{
	namespace Math3D
	{
		/*
		返回值的绝对值是四点构成四面体面积的4倍
		返回值如果小于0 则顶点D位于ABC上 从D处观察时 三角形ABC为逆时针
		如果大于0 则顶点D位于ABC下
		如果等于0 则四点共面
		*/
		template<typename Type>
		Type aOrient3D(const aVec3_t<Type>& a,
			const aVec3_t<Type>& b,
			const aVec3_t<Type>& c,
			const aVec3_t<Type>& d)
		{
			return makeMatrix<Type>(a.x - d.x, a.y - d.y, a.z - d.z,
				b.x - d.x, b.y - d.y, b.z - d.z,
				c.x - d.x, c.y - d.y, c.z - d.z).determinant();
		}
		/*
		!!!ABCD的方向要遵循逆时针方向aOrient3D(ABCD)<0，
		如果结果大于0 则E在球体之内 且ABCD位于球面上
		如果结果小于0 则E在球体之外
		如果等于0 五点共球体
		*/
		template<typename Type>
		Type aInSphere(const aVec3_t<Type>& a,
			const aVec3_t<Type>& b,
			const aVec3_t<Type>& c,
			const aVec3_t<Type>& d,
			const aVec3_t<Type>& e)
		{
			return makeMatrix(
				a.x - e.x, a.y - e.y, a.z - e.z, aSQUARE(a.x - e.x) + aSQUARE(a.y - e.y) + aSQUARE(a.z - e.z),
				b.x - e.x, b.y - e.y, b.z - e.z, aSQUARE(b.x - e.x) + aSQUARE(b.y - e.y) + aSQUARE(b.z - e.z),
				c.x - e.x, c.y - e.y, c.z - e.z, aSQUARE(c.x - e.x) + aSQUARE(c.y - e.y) + aSQUARE(c.z - e.z),
				d.x - e.x, d.y - e.y, d.z - e.z, aSQUARE(d.x - e.x) + aSQUARE(d.y - e.y) + aSQUARE(d.z - e.z))
				.determinant();
		}
	}
}
