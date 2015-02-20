#pragma once
#include"shapecategory/CollisionCircle.hpp"
#include"shapecategory/CollisionLine.hpp"
#include"shapecategory/CollisionRectangle.hpp"

namespace aMazing
{
	template<typename T>
	bool isBetweenValue(T tar, T lower, T upper)
	{
		return (tar >= lower && (tar <= upper));
	}

	/*
	*@brief 
	*judge whether a line and a circle intercect with each other
	*/
	bool IntersectLineCircle(CollisionLine* line, CollisionCircle* circle);
	
	/*
	*@brief
	*judge whether a rectangle and a circle intercect with each other
	*/
	bool IntersectRectangleCircle(CollisionRectangle* rec, CollisionCircle* circle);
}
