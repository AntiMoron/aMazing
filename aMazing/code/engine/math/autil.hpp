#pragma once

#include"amatrix.hpp"
#include"avector.hpp"

namespace aMazing
{
	template<typename type>
	aMatrix4x4_t<type> aMul(float a, const aMatrix4x4_t<type>& b)
	{
		aMatrix4x4_t<type> result;
		result.m11 = b.m11 * a; result.m12 = b.m12 * a; result.m13 = b.m13 * a; result.m14 = b.m14 * a;
		result.m21 = b.m21 * a; result.m22 = b.m22 * a; result.m23 = b.m23 * a; result.m24 = b.m24 * a;
		result.m31 = b.m31 * a; result.m32 = b.m32 * a; result.m33 = b.m33 * a; result.m34 = b.m34 * a;
		result.m41 = b.m41 * a; result.m42 = b.m42 * a; result.m43 = b.m43 * a; result.m44 = b.m44 * a;
		return result;
	}

	template<typename type>
	aMatrix4x4_t<type> aMul(const aMatrix4x4_t<type>& a, const aMatrix4x4_t<type>& b)
	{
		aMatrix4x4_t<type> result;
//		result.m11 *= a.m11;
		return result;
	}

}
