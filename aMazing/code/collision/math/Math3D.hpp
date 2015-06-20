#pragma once
#include"../../engine/math/amath.hpp"

namespace aMazing
{
	namespace Math3D
	{
		template<typename Type>
		struct aPlane
		{
			aVec3_t<Type> normal;//Plane normal.Points 'x' on the plane satisfy dot(normal,x) = d 
			Type d;//d = dot(normal,p) for a given point 'p'
		};

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
		/*
		计算质心
		*/
		template<typename Type>
		aVec3_t<Type> barycentric(cosnt aVec3_t<Type>& a,
			const aVec3_t<Type>& b,
			const aVec3_t<Type>& c,
			const aVec3_t<Type>& p)
		{
			aVec3_t<Type> v0 = b - a;
			aVec3_t<Type> v1 = c - a;
			aVec3_t<Type> v2 = p - a;
			Type d20 = v2.dot(v0);
			Type d21 = v2.dot(v1);
			//下面4个值对于同一三角形的值是不变的，可以优化
			Type d00 = v0.dot(v0);
			Type d01 = v0.dot(v1);
			Type d11 = v1.dot(v1);
			Type denom = d00 * d11 - aSQUARE(d01);

			Type v, w;
			v = (d11 * d20 - (d01*d21)) / denom;
			w = (d00 * d21 - (d01*d20)) / denom;
			return makeVec(1.0 - v - w, v, w);
		}

		template<typename Type>
		aPlane<Type> computePlane(const aVec3_t<Type>& a,
			const aVec3_t<Type>& b,
			const aVec3_t<Type>& c)
		{
			aPlane<Type> result;
			result.normal = (b - a).cross(c - a);
			result.normal.normalize();
			result.d = result.normal.dot(a);
			return result;
		}
	}
}
