#include"CollisionIntersection.hpp"

bool aMazing::IntersectLineCircle(CollisionLine* line, CollisionCircle* circle)
{
	XMFLOAT2 center = circle->getCenter();
	XMFLOAT2 ed = line->getEndPt();
	XMFLOAT2 st = line->getStartPt();
	st.x -= center.x; st.y -= center.y;
	ed.x -= center.x; ed.y -= center.y;
	XMFLOAT2 dse = { ed.x - st.x, ed.y - st.y };
	float cr = circle->getRadius();
	float det = st.x * ed.y - (st.y * ed.x);
	float dr2 = dse.x * dse.x + (dse.y * dse.y);
	float delt = dr2 * cr * cr - (det*det);
	return delt >= 0;
}


bool aMazing::IntersectRectangleCircle(CollisionRectangle* rec, CollisionCircle* circle)
{
	XMFLOAT2 center = circle->getCenter();
	std::array<XMFLOAT2, 2> arr = rec->getPoints();
	if (isBetweenValue<float>(center.x, arr[0].x, arr[1].x) &&
		isBetweenValue<float>(center.y, arr[0].y, arr[1].y))
	{
		return true;
	}
	CollisionLine line;
	line.setStartPt(arr[0]);
	line.setEndPt(XMFLOAT2(arr[0].x, arr[1].y));
	if (IntersectLineCircle(&line, circle))
	{
		return true;
	}

	line.setStartPt(arr[0]);
	line.setEndPt(XMFLOAT2(arr[1].x, arr[0].y));
	if (IntersectLineCircle(&line, circle))
	{
		return true;
	}

	line.setStartPt(arr[1]);
	line.setEndPt(XMFLOAT2(arr[0].x, arr[1].y));
	if (IntersectLineCircle(&line, circle))
	{
		return true;
	}
	line.setStartPt(arr[1]);
	line.setEndPt(XMFLOAT2(arr[1].x, arr[0].y));
	if (IntersectLineCircle(&line, circle))
	{
		return true;
	}
	return false;
}