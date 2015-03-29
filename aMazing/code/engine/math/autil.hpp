#pragma once

#include"amatrix.hpp"
#include"avector.hpp"

namespace aMazing
{
	template<typename type>
	aMatrix4x4_t<type> aMul(float a, const aMatrix4x4_t<type>& b) aNOEXCEPT
	{
		aMatrix4x4_t<type> result;
		result.m11 = b.m11 * a; result.m12 = b.m12 * a; result.m13 = b.m13 * a; result.m14 = b.m14 * a;
		result.m21 = b.m21 * a; result.m22 = b.m22 * a; result.m23 = b.m23 * a; result.m24 = b.m24 * a;
		result.m31 = b.m31 * a; result.m32 = b.m32 * a; result.m33 = b.m33 * a; result.m34 = b.m34 * a;
		result.m41 = b.m41 * a; result.m42 = b.m42 * a; result.m43 = b.m43 * a; result.m44 = b.m44 * a;
		return result;
	}

	template<typename type>
	aMatrix2x2_t<type> aMul(const aMatrix2x2_t<type>& a, const aMatrix3x3_<type>& b) aNOEXCEPT
	{
		aMatrix2x2_t<type> result;
		result.m11 = (a.m11 * b.m11) + (a.m12 * b.m21);
		result.m21 = (a.m21 * b.m11) + (a.m22 * b.m21);

		result.m12 = (a.m11 * b.m12) + (a.m12 * b.m22);
		result.m22 = (a.m21 * b.m12) + (a.m22 * b.m22);
		return result;
	}

	template<typename type>
	aMatrix3x3_t<type> aMul(const aMatrix3x3_t<type>& a, const aMatrix3x3_t<type>& b) aNOEXCEPT
	{
		aMatrix3x3_t<type> result;
		result.m11 = (a.m11 * b.m11) + (a.m12 * b.m21) + (a.m13 * b.m31);
		result.m21 = (a.m21 * b.m11) + (a.m22 * b.m21) + (a.m23 * b.m31);
		result.m31 = (a.m31 * b.m11) + (a.m32 * b.m21) + (a.m33 * b.m31);

		result.m12 = (a.m11 * b.m12) + (a.m12 * b.m22) + (a.m13 * b.m32);
		result.m22 = (a.m21 * b.m12) + (a.m22 * b.m22) + (a.m23 * b.m32);
		result.m32 = (a.m31 * b.m12) + (a.m32 * b.m22) + (a.m33 * b.m32);

		result.m13 = (a.m11 * b.m13) + (a.m12 * b.m23) + (a.m13 * b.m33);
		result.m23 = (a.m21 * b.m13) + (a.m22 * b.m23) + (a.m23 * b.m33);
		result.m33 = (a.m31 * b.m13) + (a.m32 * b.m23) + (a.m33 * b.m33);
		return result;
	}

	template<typename type>
	aMatrix4x4_t<type> aMul(const aMatrix4x4_t<type>& a, const aMatrix4x4_t<type>& b) aNOEXCEPT
	{
		aMatrix4x4_t<type> result;
		result.m11 = (a.m11 * b.m11) + (a.m12 * b.m21) + (a.m13 * b.m31) + (a.m14 * b.m41);
		result.m21 = (a.m21 * b.m11) + (a.m22 * b.m21) + (a.m23 * b.m31) + (a.m24 * b.m41);
		result.m31 = (a.m31 * b.m11) + (a.m32 * b.m21) + (a.m33 * b.m31) + (a.m34 * b.m41);
		result.m41 = (a.m41 * b.m11) + (a.m42 * b.m21) + (a.m43 * b.m31) + (a.m44 * b.m41);

		result.m12 = (a.m11 * b.m12) + (a.m12 * b.m22) + (a.m13 * b.m32) + (a.m14 * b.m42);
		result.m22 = (a.m21 * b.m12) + (a.m22 * b.m22) + (a.m23 * b.m32) + (a.m24 * b.m42);
		result.m32 = (a.m31 * b.m12) + (a.m32 * b.m22) + (a.m33 * b.m32) + (a.m34 * b.m42);
		result.m42 = (a.m41 * b.m12) + (a.m42 * b.m22) + (a.m43 * b.m32) + (a.m44 * b.m42);

		result.m13 = (a.m11 * b.m13) + (a.m12 * b.m23) + (a.m13 * b.m33) + (a.m14 * b.m43);
		result.m23 = (a.m21 * b.m13) + (a.m22 * b.m23) + (a.m23 * b.m33) + (a.m24 * b.m43);
		result.m33 = (a.m31 * b.m13) + (a.m32 * b.m23) + (a.m33 * b.m33) + (a.m34 * b.m43);
		result.m43 = (a.m41 * b.m13) + (a.m42 * b.m23) + (a.m43 * b.m33) + (a.m44 * b.m43);

		result.m14 = (a.m11 * b.m14) + (a.m12 * b.m24) + (a.m13 * b.m34) + (a.m14 * b.m44);
		result.m24 = (a.m21 * b.m14) + (a.m22 * b.m24) + (a.m23 * b.m34) + (a.m24 * b.m44);
		result.m34 = (a.m31 * b.m14) + (a.m32 * b.m24) + (a.m33 * b.m34) + (a.m34 * b.m44);
		result.m44 = (a.m41 * b.m14) + (a.m42 * b.m24) + (a.m43 * b.m34) + (a.m44 * b.m44);
		return result;
	}

	template<typename type>
	aVec2_t<type> aMul(const aMatrix2x2_t<type>& a, const aVec2_t<type>& b) aNOEXCEPT
	{
		aVec4_t<type> result;
		result.x = (a.m11 * b.x) + (a.m12 * b.y);
		result.y = (a.m21 * b.x) + (a.m22 * b.y);
		return result;
	}

	template<typename type>
	aVec2_t<type> aMul(const aVec2_t<type>& b, const aMatrix2x2_t<type>& a) aNOEXCEPT
	{
		aVec4_t<type> result;
		result.x = (a.m11 * b.x) + (a.m21 * b.y);
		result.y = (a.m12 * b.x) + (a.m22 * b.y);
		return result;
	}

	template<typename type>
	aVec2_t<type> aMul(const aMatrix3x3_t<type>& a, const aVec3_t<type>& b) aNOEXCEPT
	{
		aVec4_t<type> result;
		result.x = (a.m11 * b.x) + (a.m12 * b.y) + (a.m13 * b.z);
		result.y = (a.m21 * b.x) + (a.m22 * b.y) + (a.m23 * b.z);
		result.z = (a.m31 * b.x) + (a.m32 * b.y) + (a.m33 * b.z);
		return result;
	}

	template<typename type>
	aVec3_t<type> aMul(const aVec3_t<type>& b, const aMatrix3x3_t<type>& a) aNOEXCEPT
	{
		aVec4_t<type> result;
		result.x = (a.m11 * b.x) + (a.m21 * b.y) + (a.m31 * b.z);
		result.y = (a.m12 * b.x) + (a.m22 * b.y) + (a.m32 * b.z);
		result.z = (a.m13 * b.x) + (a.m23 * b.y) + (a.m33 * b.z);
		return result;
	}

	template<typename type>
	aVec4_t<type> aMul(const aMatrix4x4_t<type>& a, const aVec4_t<type>& b) aNOEXCEPT
	{
		aVec4_t<type> result;
		result.x = (a.m11 * b.x) + (a.m12 * b.y) + (a.m13 * b.z) + (a.m14 * b.w);
		result.y = (a.m21 * b.x) + (a.m22 * b.y) + (a.m23 * b.z) + (a.m24 * b.w);
		result.z = (a.m31 * b.x) + (a.m32 * b.y) + (a.m33 * b.z) + (a.m34 * b.w);
		result.w = (a.m41 * b.x) + (a.m42 * b.y) + (a.m43 * b.z) + (a.m44 * b.w);
		return result;
	}

	template<typename type>
	aVec4_t<type> aMul(const aVec4_t<type>& b, const aMatrix4x4_t<type>& a) aNOEXCEPT
	{
		aVec4_t<type> result;
		result.x = (a.m11 * b.x) + (a.m21 * b.y) + (a.m31 * b.z) + (a.m41 * b.w);
		result.y = (a.m12 * b.x) + (a.m22 * b.y) + (a.m32 * b.z) + (a.m42 * b.w);
		result.z = (a.m13 * b.x) + (a.m23 * b.y) + (a.m33 * b.z) + (a.m43 * b.w);
		result.w = (a.m14 * b.x) + (a.m24 * b.y) + (a.m34 * b.z) + (a.m44 * b.w);
		return result;
	}
}
