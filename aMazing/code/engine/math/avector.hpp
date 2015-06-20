#pragma once
#include"../../common/CommonDef.hpp"
#include"../../common/CommonTemplates.hpp"
#include<type_traits>
#include<cmath>
namespace aMazing
{
	namespace reserve
	{
		//This struct is reserved for disabling non numeric type as the template argument.
		template<typename T,typename Enable = void>
		struct baseVec_t{};

		template<typename T>
		struct baseVec_t<T, typename std::enable_if<aIsNumeric<T>::value >::value >{};
	}

	template<typename type>
	struct aVec2_t : public reserve::baseVec_t<type>
	{
		type x;
		type y;
		//make all the elements in the vector be 1.0f
		void beOne() aNOEXCEPT
		{
			x = 1.0f;
			y = 1.0f;
		}
		//make all the elements in the vector be 0.0f
		void beZero() aNOEXCEPT
		{
			x = 0.0f;
			y = 0.0f;
		}
		//check whether the vector is zero.
		const bool isZero() const
		{
			return x == 0.0f && y == 0.0f;
		}
		//return the size of the vector
		float size() const
		{
			return std::sqrt((x * x) + (y * y));
		}

		//normalize the vector
		void normalize() aNOEXCEPT
		{
			if (isZero())
			{
				return ;
			}
			float vecSize = size();
			x /= vecSize;
			y /= vecSize;
		}

		//To do dot calculation
		type dot(const aVec2_t<type>& other) const aNOEXCEPT
		{
			return (x * other.x) + (y * other.y);
		}
		//To do cross calculation
		type cross(const aVec2_t<type>& other) const aNOEXCEPT
		{
			return x * other.y - (y * other.x);
		}
		//To get the length of the vector.
		float size()
		{
			return std::sqrt(x * x + y * y);
		}

		const bool operator == (const aVec2_t<type>& other) const aNOEXCEPT
		{
			return (x == other.x) && (y == other.y);
		}
		aVec2_t<type> operator + (const aVec2_t<type>& other)const aNOEXCEPT
		{
			aVec4_t<type> result;
			result.x = x + other.x;
			result.y = y + other.y;
			return result;
		}

		aVec2_t<type> operator - (const aVec2_t<type>& other)const aNOEXCEPT
		{
			aVec4_t<type> result;
			result.x = x - other.x;
			result.y = y - other.y;
			return result;
		}
	};

	template<typename type>
	struct aVec3_t : public reserve::baseVec_t<type>
	{
		type x;
		type y;
		type z;
		//make all the elements in the vector be 1.0f
		void beOne() aNOEXCEPT
		{
			x = 1.0f;
			y = 1.0f;
			z = 1.0f;
		}
		//make all the elements in the vector be 0.0f
		void beZero() aNOEXCEPT
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}
		//check whether the vector is zero.
		const bool isZero() const
		{
			return x == 0.0f && y == 0.0f && z == 0.0f;
		}
		//return the size of the vector
		float size() const
		{
			return std::sqrt((x * x) + (y * y) + (z * z));
		}

		//normalize the vector
		void normalize() aNOEXCEPT
		{
			if (isZero())
			{
				return;
			}
			float vecSize = size();
			x /= vecSize;
			y /= vecSize;
			z /= vecSize;
		}
		//To do dot calculation
		double dot(const aVec3_t<type>& other) const aNOEXCEPT
		{
			return (x * other.x) + (y * other.y) + (z * other.z);
		}

		//To do cross calculat
		aVec3_t<type> cross(const aVec3_t<type>& other)const aNOEXCEPT
		{
			aVec3_t<type> result;
			result.x = y * other.z - (z * other.y);
			result.y = z * other.x - (x * other.z);
			result.z = x * other.y - (y * other.x);
			return result;
		}

		const bool operator == (const aVec3_t<type>& other) const aNOEXCEPT
		{
			return (x == other.x) && (y == other.y) && (z == other.z);
		}
		aVec3_t<type> operator + (const aVec3_t<type>& other)const aNOEXCEPT
		{
			aVec4_t<type> result;
			result.x = x + other.x;
			result.y = y + other.y;
			result.z = z + other.z;
			return result;
		}

		aVec3_t<type> operator - (const aVec3_t<type>& other)const aNOEXCEPT
		{
			aVec4_t<type> result;
			result.x = x - other.x;
			result.y = y - other.y;
			result.z = z - other.z;
			return result;
		}
	};

	template<typename type>
	struct aVec4_t : public reserve::baseVec_t<type>
	{
		type x;
		type y;
		type z;
		type w;
		//make all the elements in the vector be 1.0f
		void beOne() aNOEXCEPT
		{
			x = 1.0f;
			y = 1.0f;
			z = 1.0f;
			w = 1.0f;
		}
		//make all the elements in the vector be 0.0f
		void beZero() aNOEXCEPT
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
		}
		//check whether the vector is zero.
		const bool isZero() const
		{
			return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
		}
		//return the size of the vector
		float size() const
		{
			return std::sqrt((x * x) + (y * y) + (z * z) + (w * w));
		}

		//normalize the vector
		void normalize() aNOEXCEPT
		{
			if (isZero())
			{
				return;
			}
			float vecSize = size();
			x /= vecSize;
			y /= vecSize;
			z /= vecSize;
			w /= vecSize;
		}
		//To do dot calculation
		double dot(const aVec4_t<type>& other) const aNOEXCEPT
		{
			return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
		}
		const bool operator == (const aVec4_t<type>& other) const aNOEXCEPT
		{
			return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
		}
		aVec4_t<type> operator + (const aVec4_t<type>& other)const aNOEXCEPT
		{
			aVec4_t<type> result;
			result.x = x + other.x;
			result.y = y + other.y;
			result.z = z + other.z;
			result.w = w + other.w;
			return result;
		}

		aVec4_t<type> operator - (const aVec4_t<type>& other)const aNOEXCEPT
		{
			aVec4_t<type> result;
			result.x = x - other.x;
			result.y = y - other.y;
			result.z = z - other.z;
			result.w = w - other.w;
			return result;
		}
	};

	template<typename type>
	aVec2_t<type> aMazing::makeVec(type x, type y)
	{
		aVec2_t<type> result;
		result.x = x;
		result.y = y;
		return result;
	}

	template<typename type>
	aVec3_t<type> aMazing::makeVec(type x, type y, type z)
	{
		aVec3_t<type> result;
		result.x = x;
		result.y = y;
		result.z = z;
		return result;
	}

	template<typename type>
	aVec4_t<type> aMazing::makeVec(type x, type y, type z, type w)
	{
		aVec4_t<type> result;
		result.x = x;
		result.y = y;
		result.z = z;
		result.w = w;
		return result;
	}

	typedef aVec2_t<int> aIntVec2;
	typedef aVec2_t<float> aFloatVec2;
	typedef aVec2_t<double> aDoubleVec2;
	typedef aVec3_t<int> aIntVec3;
	typedef aVec3_t<float> aFloatVec3;
	typedef aVec3_t<double> aDoubleVec3;
	typedef aVec4_t<int> aIntVec4;
	typedef aVec4_t<float> aFloatVec4;
	typedef aVec4_t<double> aDoubleVec4;
}
