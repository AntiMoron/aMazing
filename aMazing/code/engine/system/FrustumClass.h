#pragma once
#include<D3D11.h>
#include<D3DX11.h>
#include<xnamath.h>
#include<D3DX10math.h>
class FrustumClass
{
public:
	FrustumClass(void);
	~FrustumClass(void);
	bool CheckPoint(float x, float y, float z);
	bool CheckCube(float xCenter, float yCenter, float zCenter, float radius_x, float radius_y, float radius_z);
	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
private:
	friend class CameraClass;
	void ConstructFrustum(float screenDepth, XMMATRIX& projection, XMMATRIX& view);
	XMVECTOR m_planes[6];
};

