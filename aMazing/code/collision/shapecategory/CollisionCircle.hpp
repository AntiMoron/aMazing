#pragma once
#include"../CollisionShape.hpp"
namespace aMazing
{
	class CollisionCircle : public CollisionShape
	{
	public:
		CollisionCircle()
		{
			type = shape_circle;
		}
		inline XMFLOAT2 getCenter()
		{
			return center;
		}

		void setCenter(XMFLOAT2 center)
		{
			this->center = center;
		}

		inline float getRadius()
		{
			return radius;
		}

		void setRadius(float radius)
		{
			this->radius = radius;
		}
	private:
		XMFLOAT2 center;
		float radius;
	};
}
