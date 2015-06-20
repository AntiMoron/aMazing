#pragma once
#include"../../engine/math/amath.hpp"

namespace aMazing
{
	namespace Math3D
	{
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
	}
}
