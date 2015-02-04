#pragma once
#include"shapecategory/CollisionCircle.hpp"
#include"shapecategory/CollisionLine.hpp"
#include"shapecategory/CollisionRectangle.hpp"

template<typename T>
bool isBetweenValue(T tar, T lower, T upper)
{
	return (tar >= lower && (tar <= upper));
}
bool IntersectLineCircle(CollisionLine* line, CollisionCircle* circle);
bool IntersectRectangleCircle(CollisionRectangle* rec, CollisionCircle* circle);

