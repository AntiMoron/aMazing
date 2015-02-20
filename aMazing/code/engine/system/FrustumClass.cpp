#include "FrustumClass.hpp"
using namespace aMazing;

FrustumClass::FrustumClass(void)
{
}


FrustumClass::~FrustumClass(void)
{
}

void FrustumClass::ConstructFrustum(float screenDepth, XMMATRIX& projectionMatrix, XMMATRIX& viewMatrix)
{
	float zMinimum, r;

	// Calculate the minimum Z distance in the frustum.
	zMinimum = -projectionMatrix._43 / projectionMatrix._33;
	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.

	XMMATRIX stackViewMatrix = viewMatrix;
	XMMATRIX stackProjectionMatrix = projectionMatrix;
	XMMATRIX matrix = stackViewMatrix * stackProjectionMatrix;

	// Calculate near plane of frustum.
	m_planes[0].m128_f32[0] = matrix._14 + matrix._13;
	m_planes[0].m128_f32[1] = matrix._24 + matrix._23;
	m_planes[0].m128_f32[2] = matrix._34 + matrix._33;
	m_planes[0].m128_f32[3] = matrix._44 + matrix._43;
	m_planes[0] = XMPlaneNormalize(m_planes[0]);

	// Calculate far plane of frustum.
	m_planes[1].m128_f32[0] = matrix._14 - matrix._13;
	m_planes[1].m128_f32[1] = matrix._24 - matrix._23;
	m_planes[1].m128_f32[2] = matrix._34 - matrix._33;
	m_planes[1].m128_f32[3] = matrix._44 - matrix._43;
	m_planes[1] = XMPlaneNormalize(m_planes[1]);

	// Calculate left plane of frustum.
	m_planes[2].m128_f32[0] = matrix._14 + matrix._11;
	m_planes[2].m128_f32[1] = matrix._24 + matrix._21;
	m_planes[2].m128_f32[2] = matrix._34 + matrix._31;
	m_planes[2].m128_f32[3] = matrix._44 + matrix._41;
	m_planes[2] = XMPlaneNormalize(m_planes[2]);

	// Calculate right plane of frustum.
	m_planes[3].m128_f32[0] = matrix._14 - matrix._11;
	m_planes[3].m128_f32[1] = matrix._24 - matrix._21;
	m_planes[3].m128_f32[2] = matrix._34 - matrix._31;
	m_planes[3].m128_f32[3] = matrix._44 - matrix._41;
	m_planes[3] = XMPlaneNormalize(m_planes[3]);

	// Calculate top plane of frustum.
	m_planes[4].m128_f32[0] = matrix._14 - matrix._12;
	m_planes[4].m128_f32[1] = matrix._24 - matrix._22;
	m_planes[4].m128_f32[2] = matrix._34 - matrix._32;
	m_planes[4].m128_f32[3] = matrix._44 - matrix._42;
	m_planes[4] = XMPlaneNormalize(m_planes[4]);

	// Calculate bottom plane of frustum.
	m_planes[5].m128_f32[0] = matrix._14 + matrix._12;
	m_planes[5].m128_f32[1] = matrix._24 + matrix._22;
	m_planes[5].m128_f32[2] = matrix._34 + matrix._32;
	m_planes[5].m128_f32[3] = matrix._44 + matrix._42;
	m_planes[5] = XMPlaneNormalize(m_planes[5]);
}

bool FrustumClass::CheckPoint(float x, float y, float z)
{
	int i;

	XMVECTOR tester = {x,y,z};
	// Check if the point is inside all six planes of the view frustum.
	for (i = 0; i < 6; i++)
	{
		XMVECTOR ret = XMPlaneDotCoord(m_planes[i], tester);
		float dotRet = (ret.m128_f32[0] + ret.m128_f32[1] + ret.m128_f32[2] + ret.m128_f32[3]);
		if (dotRet < 0.0f)
		{
			return false;
		}
	}

	return true;
}
bool FrustumClass::CheckCube(float xCenter, float yCenter, float zCenter, float radius_x, float radius_y, float radius_z)
{
	int i;

	XMVECTOR ret;
	XMVECTOR tester[] = {
		{ (xCenter - radius_x), (yCenter - radius_y), (zCenter - radius_z) },
		{ (xCenter - radius_x), (yCenter - radius_y), (zCenter + radius_z) },
		{ (xCenter - radius_x), (yCenter + radius_y), (zCenter - radius_z) },
		{ (xCenter - radius_x), (yCenter + radius_y), (zCenter + radius_z) },
		{ (xCenter + radius_x), (yCenter - radius_y), (zCenter - radius_z) },
		{ (xCenter + radius_x), (yCenter - radius_y), (zCenter + radius_z) },
		{ (xCenter + radius_x), (yCenter + radius_y), (zCenter - radius_z) },
		{ (xCenter + radius_x), (yCenter + radius_y), (zCenter + radius_z) }
	};

	
	// Check if any one point of the cube is in the view frustum.
	for (auto& t : tester)
	{
		if (CheckPoint(t.m128_f32[0], t.m128_f32[1], t.m128_f32[2]))
		{
			return true;
		}
	}
	return false;
}

bool FrustumClass::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;

	XMVECTOR tester = { xCenter, yCenter, zCenter };
	XMVECTOR ret;
	// Check if the radius of the sphere is inside the view frustum.
	for (i = 0; i<6; i++)
	{
		ret = XMPlaneDotCoord(m_planes[i], tester);
		if ((ret.m128_f32[0] + ret.m128_f32[1] + ret.m128_f32[2] + ret.m128_f32[3]) < -radius)
		{
			return false;
		}
	}

	return true;
}