#pragma once
#include"CollisionShape.hpp"
#include"CollisionCircle.hpp"

class CollisionLine : public CollisionShape
{
public:
	CollisionLine()
	{
		type = shape_line;
	}
	void setStartPt(XMFLOAT2 st)
	{
		startPt = st;
	}
	XMFLOAT2 getStartPt()
	{
		return startPt;
	}

	void setEndPt(XMFLOAT2 ed)
	{
		endPt = ed;
	}

	XMFLOAT2 getEndPt()
	{
		return endPt;
	}
private:
	XMFLOAT2 startPt;
	XMFLOAT2 endPt;
};