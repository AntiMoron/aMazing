#pragma once
#include<array>
#include"../CollisionShape.hpp"

namespace aMazing
{
	class CollisionRectangle :public CollisionShape
	{
	public:
		CollisionRectangle()
		{
			type = shape_rectangle;
		}
		void setPoints(std::array<XMFLOAT2, 2> pts)
		{
			points = pts;
		}
		std::array<XMFLOAT2, 2> getPoints()
		{
			return points;
		}
	private:
		std::array<XMFLOAT2, 2> points;
	};
}
