#pragma once
#include"../common/CommonDxSupport.hpp"
#include<vector>
#include<xnamath.h>
class CollisionShape
{
public:
	typedef enum shapeType
	{
		shape_undef,
		shape_circle,
		shape_rectangle,
		shape_triangle,
		shape_line,
		shape_polygon
	}shapeType;

	shapeType getShapeType()
	{
		return type;
	}
protected:
	shapeType type;
};