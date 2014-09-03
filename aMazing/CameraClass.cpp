#include "CameraClass.h"


CameraClass::CameraClass()
{
}


CameraClass::~CameraClass()
{
}


HRESULT CameraClass::Initialize(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	position = { 0.0f, 0.0f, 0.0f };
	rotation = { 0.0f, 0.0f, 0.0f };
	upVector = { 0.0f, 1.0f, 0.0f };
	
	setFov(40);
	near_far = {0.001f,1000.0f};

	D3DXMATRIX rotationMatrix;
	D3DXVECTOR3 lookDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 upDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.z, rotation.y, rotation.x);
	D3DXVec3TransformCoord(&lookDir, &lookDir, &rotationMatrix);
	D3DXVec3Normalize(&lookDir, &lookDir);
	D3DXVec3TransformCoord(&upDir, &upDir, &rotationMatrix);
	D3DXVec3Normalize(&upDir, &upDir);

	FXMVECTOR posVector = { position.x, position.y, position.z };
	FXMVECTOR focVector = { position.x + lookDir.x,
		position.y + lookDir.y,
		position.z + lookDir.z };
	FXMVECTOR _upVector = { upDir.x, upDir.y, upDir.z };
	upVector = { upDir.x, upDir.y, upDir.z };
	m_matriceData.world = XMMatrixIdentity();
	m_matriceData.view = XMMatrixLookAtLH(posVector, focVector, _upVector);
	m_matriceData.projection = XMMatrixPerspectiveFovLH(fov, 16.0 / 9, near_far.x, near_far.y);

	m_matriceData.world = XMMatrixTranspose(m_matriceData.world);
	m_matriceData.view = XMMatrixTranspose(m_matriceData.view);
	m_matriceData.projection = XMMatrixTranspose(m_matriceData.projection);
	//Update Buffer
	m_matrices.Initialize(device, context, 0);
	m_matrices.UpdateData(&m_matriceData);
	m_matrices.UpdateGpu(device, context);
	m_matrices.BindVertexShader(device, context);
	return S_OK;
}

void CameraClass::Shutdown()
{
	m_matrices.Shutdown();
}

void CameraClass::setPosition(const XMFLOAT3& pos)
{
	position = pos;
}

void CameraClass::setPosition(const XMFLOAT3&& pos)
{
	position = pos;
}

XMFLOAT3 CameraClass::getPosition() const 
{
	return position;
}

void CameraClass::setRotation(const XMFLOAT3& rot)
{
	rotation = rot;
}

void CameraClass::setRotation(const XMFLOAT3&& rot)
{
	rotation = rot;
}

XMFLOAT3 CameraClass::getRotation() const
{
	return rotation;
}

XMFLOAT3 CameraClass::getUpVector()const
{
	return upVector;
}

void CameraClass::setNearDist(const float val)
{
	near_far.x = val;
}

void CameraClass::setFarDist(const float val)
{
	near_far.y = val;
}

float CameraClass::getNearDist() const
{
	return near_far.x;
}

float CameraClass::getFarDist() const
{
	return near_far.y;
}

void CameraClass::setFov(const float ang)
{
	fov = ang * 0.017453292f;
}

float CameraClass::getFov()const
{
	return fov / 0.017453292f;
}

void CameraClass::Render(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	D3DXMATRIX rotationMatrix;
	D3DXVECTOR3 lookDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 upDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix,rotation.z,rotation.y,rotation.x);
	D3DXVec3TransformCoord(&lookDir,&lookDir,&rotationMatrix);
	D3DXVec3Normalize(&lookDir, &lookDir);
	D3DXVec3TransformCoord(&upDir, &upDir, &rotationMatrix);
	D3DXVec3Normalize(&upDir, &upDir);

	FXMVECTOR posVector = { position.x, position.y, position.z };
	FXMVECTOR focVector = { position.x + lookDir.x, 
		position.y + lookDir.y,
		position.z + lookDir.z };
	FXMVECTOR _upVector = { upDir.x, upDir.y, upDir.z };
	upVector = { upDir.x, upDir.y, upDir.z };
	m_matriceData.world = XMMatrixIdentity();
	m_matriceData.view = XMMatrixLookAtLH(posVector, focVector, _upVector);
	m_matriceData.projection = XMMatrixPerspectiveFovLH(fov,16.0/9,near_far.x,near_far.y);

	m_matriceData.world = XMMatrixTranspose(m_matriceData.world);
	m_matriceData.view = XMMatrixTranspose(m_matriceData.view);
	m_matriceData.projection = XMMatrixTranspose(m_matriceData.projection);
	m_matrices.UpdateData(&m_matriceData);
	m_matrices.UpdateGpu(device, context);
	m_matrices.BindVertexShader(device, context);
}