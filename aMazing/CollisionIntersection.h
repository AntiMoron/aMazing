#pragma once
#include"CollisionCircle.hpp"
#include"CollisionLine.hpp"
#include"CollisionRectangle.hpp"

template<typename T>
bool isBetweenValue(T tar, T lower, T upper)
{
	return (tar >= lower && (tar <= upper));
}
bool IntersectLineCircle(CollisionLine* line, CollisionCircle* circle);
bool IntersectRectangleCircle(CollisionRectangle* rec, CollisionCircle* circle);

