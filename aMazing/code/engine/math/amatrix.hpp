#pragma once

#include"../../common/CommonDef.hpp"
#include"../../common/CommonTemplates.hpp"

namespace aMazing
{
	namespace reserve
	{
		//This struct is reserved for disabling non numeric type as the template argument.
		template<typename T, typename Enable = void>
		struct baseMatrix_t{};

		template<typename T>
		struct baseMatrix_t<T,
			typename std::enable_if<aIsNumeric<T>::value, T>::type >{};
	}

	template<typename type>
	struct aMatrix2x2_t : public reserve::baseMatrix_t<type>
	{
		type m11, m12, m21, m22;
		//To transpose the matrix
		void transpose() aNOEXCEPT
		{
			aSwap(m12, m21);
		}

		//To make the matrix zero.
		void beZero() aNOEXCEPT
		{
			m11 = 0; m12 = 0;
			m21 = 0; m22 = 0;
		}
		//To make the matrix identity matrix.
		void beIdentity() aNOEXCEPT
		{
			m11 = 1; m12 = 0;
			m21 = 0; m22 = 1;
		}
		//return whether the matrix is identity matrix.
		const bool isIdentity() const aNOEXCEPT
		{
			if (m11 == 1 && m22 == m11)
			{
				if (m12 && m21)
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			return false;
		}
		//return whether the matrix is zero.
		const bool isZero() const aNOEXCEPT
		{
			if (m11 && m12 &&
			m21 && m22)
				return false;
			return true;
		}
		//return whether two matrices are same.
		bool operator == (const aMatrix2x2_t& other) const aNOEXCEPT
		{
			return m11 == other.m11 && m12 == other.m12 &&
			m21 == other.m21 && m22 == other.m22;
		}
		aMatrix2x2_t<type> operator + (const aMatrix2x2_t<type>& other)const aNOEXCEPT
		{
			aMatrix2x2_t<type> result;
			result.m11 = m11 + other.m11;
			result.m12 = m12 + other.m12;

			result.m21 = m21 + other.m21;
			result.m22 = m22 + other.m22;
			return result;
		}
		aMatrix2x2_t<type> operator - (const aMatrix2x2_t<type>& other)const aNOEXCEPT
		{
			aMatrix2x2_t<type> result;
			result.m11 = m11 - other.m11;
			result.m12 = m12 - other.m12;

			result.m21 = m21 - other.m21;
			result.m22 = m22 - other.m22;
			return result;
		}
		//To get the determinant of the matrix.
		type determinant() const aNOEXCEPT
		{
			return m11 * m22 - (m12 * m21);
		}
	};

	template<typename type>
	struct aMatrix3x3_t : public reserve::baseMatrix_t<type>
	{
		type m11, m12, m13,
		m21, m22, m23,
		m31, m32, m33;
		//To transpose the matrix
		void transpose() aNOEXCEPT
		{
			aSwap(m12, m21);
			aSwap(m13, m31);
			aSwap(m23, m32);
		}

		//To make the matrix zero.
		void beZero() aNOEXCEPT
		{
			m11 = 0; m12 = 0; m13 = 0;
			m21 = 0; m22 = 0; m23 = 0;
			m31 = 0; m32 = 0; m33 = 0;
		}
		//To make the matrix identity matrix.
		void beIdentity() aNOEXCEPT
		{
			m11 = 1; m12 = 0; m13 = 0;
			m21 = 0; m22 = 1; m23 = 0;
			m31 = 0; m32 = 0; m33 = 1;
		}
		//return whether the matrix is identity matrix.
		const bool isIdentity() const aNOEXCEPT
		{
			if (m11 == 1 && m22 == m11 && m33 == m22)
			{
				if (m12 && m13 &&
					m21 && m23 &&
					m31 && m32)
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			return false;
		}
		//return whether the matrix is zero.
		const bool isZero() const aNOEXCEPT
		{
			if (m11 && m12 && m13 &&
			m21 && m22 && m23 &&
			m31 && m32 && m33)
				return false;
			return true;
		}
		//return whether two matrices are same.
		bool operator == (const aMatrix3x3_t& other) const aNOEXCEPT
		{
			return m11 == other.m11 && m12 == other.m12 && m13 == other.m13 &&
			m21 == other.m21 && m22 == other.m22 && m23 == other.m23 &&
			m31 == other.m31 && m32 == other.m32 && m33 == other.m33;
		}
		aMatrix3x3_t<type> operator + (const aMatrix3x3_t<type>& other)const aNOEXCEPT
		{
			aMatrix3x3_t<type> result;
			result.m11 = m11 + other.m11;
			result.m12 = m12 + other.m12;
			result.m13 = m13 + other.m13;

			result.m21 = m21 + other.m21;
			result.m22 = m22 + other.m22;
			result.m23 = m23 + other.m23;

			result.m31 = m31 + other.m31;
			result.m32 = m32 + other.m32;
			result.m33 = m33 + other.m33;
			return result;
		}
		aMatrix3x3_t<type> operator - (const aMatrix3x3_t<type>& other)const aNOEXCEPT
		{
			aMatrix3x3_t<type> result;
			result.m11 = m11 - other.m11;
			result.m12 = m12 - other.m12;
			result.m13 = m13 - other.m13;

			result.m21 = m21 - other.m21;
			result.m22 = m22 - other.m22;
			result.m23 = m23 - other.m23;

			result.m31 = m31 - other.m31;
			result.m32 = m32 - other.m32;
			result.m33 = m33 - other.m33;
			return result;
		}
		//To get the determinant of the matrix.
		type determinant() const aNOEXCEPT
		{
			return (m11 * ((m22 * m33) - (m23 * m32))) -
			(m12 * ((m21 * m33) - (m23 * m31))) + 
			(m13 * ((m21 * m32) - (m22 * m31)));
		}
	};

	template<typename type>
	struct aMatrix4x4_t : public reserve::baseMatrix_t<type>
	{
		type m11, m12, m13, m14,
		m21, m22, m23, m24,
		m31, m32, m33, m34,
		m41, m42, m43, m44;
		//To transpose the matrix
		void transpose() aNOEXCEPT
		{ 
			aSwap(m12, m21);
			aSwap(m13, m31);
			aSwap(m14, m41);
			aSwap(m23, m32);
			aSwap(m24, m42);
			aSwap(m34, m43);
		}
		//To make the matrix zero.
		void beZero() aNOEXCEPT
		{
			m11 = 0; m12 = 0; m13 = 0; m14 = 0;
			m21 = 0; m22 = 0; m23 = 0; m24 = 0;
			m31 = 0; m32 = 0; m33 = 0; m34 = 0;
			m41 = 0; m42 = 0; m43 = 0; m44 = 0;
		}
		//To make the matrix identity matrix.
		void beIdentity() aNOEXCEPT
		{
			m11 = 1; m12 = 0; m13 = 0; m14 = 0;
			m21 = 0; m22 = 1; m23 = 0; m24 = 0;
			m31 = 0; m32 = 0; m33 = 1; m34 = 0;
			m41 = 0; m42 = 0; m43 = 0; m44 = 1;
		}
		//return whether the matrix is identity matrix.
		const bool isIdentity() const aNOEXCEPT
		{
			if (m11 == 1 && m22 == m11 && m33 == m22 && m44 == m33)
			{
				if (m12 && m13 && m14 && 
					m21 && m23 && m24 && 
					m31 && m32 && m34 && 
					m41 && m42 && m43)
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			return false;
		}
		//return whether the matrix is zero.
		const bool isZero() const aNOEXCEPT
		{
			if (m11 && m12 && m13 && m14 &&
			m21 && m22 && m23 && m24 &&
			m31 && m32 && m33 && m34 &&
			m41 && m42 && m43 && m44)
				return false;
			return true;
		}
		//return whether two matrices are same.
		bool operator == (const aMatrix4x4_t& other) const aNOEXCEPT
		{
			return m11 == other.m11 && m12 == other.m12 && m13 == other.m13 && m14 == other.m14 &&
			m21 == other.m21 && m22 == other.m22 && m23 == other.m23 && m24 == other.m24 &&
			m31 == other.m31 && m32 == other.m32 && m33 == other.m33 && m34 == other.m34 && 
			m41 == other.m41 && m42 == other.m42 && m43 == other.m43 && m44 == other.m44;
		}

		aMatrix4x4_t<type> operator + (const aMatrix4x4_t<type>& other)const aNOEXCEPT
		{
			aMatrix4x4_t<type> result;
			result.m11 = m11 + other.m11;
			result.m12 = m12 + other.m12;
			result.m13 = m13 + other.m13;
			result.m14 = m14 + other.m14;

			result.m21 = m21 + other.m21;
			result.m22 = m22 + other.m22;
			result.m23 = m23 + other.m23;
			result.m24 = m24 + other.m24;

			result.m31 = m31 + other.m31;
			result.m32 = m32 + other.m32;
			result.m33 = m33 + other.m33;
			result.m34 = m34 + other.m34;

			result.m41 = m41 + other.m41;
			result.m42 = m42 + other.m42;
			result.m43 = m43 + other.m43;
			result.m44 = m44 + other.m44;
			return result;
		}
		aMatrix4x4_t<type> operator - (const aMatrix4x4_t<type>& other)const aNOEXCEPT
		{
			aMatrix4x4_t<type> result;
			result.m11 = m11 - other.m11;
			result.m12 = m12 - other.m12;
			result.m13 = m13 - other.m13;
			result.m21 = m21 - other.m21;
			result.m22 = m22 - other.m22;
			result.m24 = m23 - other.m23;
			result.m31 = m31 - other.m31;
			result.m32 = m32 - other.m32;
			result.m33 = m33 - other.m33;
			return result;
		}

		//To get the determinant of the matrix.
		type determinant() const aNOEXCEPT
		{
			aMatrix3x3_t<type> mat1 = { m22, m23, m24, m32, m33, m34, m42, m43, m44 };
			aMatrix3x3_t<type> mat2 = { m21, m23, m24, m31, m33, m34, m41, m43, m44 };
			aMatrix3x3_t<type> mat3 = { m21, m22, m24, m31, m32, m34, m41, m42, m44 };
			aMatrix3x3_t<type> mat4 = { m21, m22, m23, m31, m32, m33, m41, m42, m43 };
			return m11 * mat1.determinant() - (m12 * mat2.determinant()) + 
				(m13 * mat3.determinant()) - (m14 * mat4.determinant());
		}
	};

	template<typename type>
	aMatrix2x2_t<type> makeMatrix(type a11, type a12,
		type a21, type a22)
	{
		aMatrix2x2_t<type> result;
		result.m11 = a11; result.m12 = a12;
		result.m21 = a21; result.m22 = a22;
		return result;
	}

	template<typename type>
	aMatrix3x3_t<type> makeMatrix(type a11, type a12, type a13,
		type a21, type a22, type a23,
		type a31, type a32, type a33)
	{
		aMatrix3x3_t<type> result;
		result.m11 = a11; result.m12 = a12; result.m13 = a13;
		result.m21 = a21; result.m22 = a22; result.m23 = a23;
		result.m31 = a31; result.a32 = a32; result.a33 = a33;
		return result;
	}

	template<typename type>
	aMatrix4x4_t<type> makeMatrix(type a11, type a12, type a13, type a14,
		type a21, type a22, type a23, type a24,
		type a31, type a32, type a33, type a34,
		type a41, type a42, type a43, type a44)
	{
		aMatrix4x4_t<type> result;
		result.m11 = a11; result.m12 = a12; result.m13 = a13; result.m14 = a14;
		result.m21 = a21; result.m22 = a22; result.m23 = a23; result.m24 = a24;
		result.m31 = a31; result.m32 = a32; result.m33 = a33; result.m34 = a34;
		result.m41 = a41; result.m42 = a42; result.m43 = a43; result.m44 = a44;
		return result;
	}

	typedef aMatrix4x4_t<int> aIntMatrix4x4;
	typedef aMatrix4x4_t<float> aFloatMatrix4x4;
	typedef aMatrix4x4_t<double> aDoubleMatrix4x4;
}
