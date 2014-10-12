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
	
	forwardDirection = { 0.0f, 0.0f, -1.0f };
	lefthandDirection = { 1.0f, 0.0f, 0.0f };

	setFov(40.0f);
	near_far = { 0.000004f, 100.0f };

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
	m_matriceData.projection = XMMatrixPerspectiveFovLH(fov, ASPECTRATIO, near_far.x, near_far.y);

	decltype(m_matriceData) cbData = m_matriceData;
	cbData.world = XMMatrixTranspose(cbData.world);
	cbData.view = XMMatrixTranspose(cbData.view);
	cbData.projection = XMMatrixTranspose(cbData.projection);

	//Update Buffer
	m_matrices.Initialize(device, context, 0);
	m_matrices.UpdateData(&cbData);
	m_matrices.UpdateGpu(device, context);
	m_matrices.BindVertexShader(device, context);

	m_frustum.reset(new FrustumClass);
	m_frustum->ConstructFrustum(near_far.y,m_matriceData.projection,m_matriceData.view);
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
	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.z, rotation.y, rotation.x);
	D3DXVECTOR3 forward = { 0.0f, 0.0f, -1.0f };
	D3DXVECTOR3 lefthand = { 1.0f, 0.0f, 0.0f };
	D3DXVec3TransformCoord(&forward, &forward, &rotationMatrix);
	D3DXVec3Normalize(&forward, &forward);
	D3DXVec3TransformCoord(&lefthand, &lefthand, &rotationMatrix);
	D3DXVec3Normalize(&lefthand, &lefthand);
	forwardDirection = { forward.x, forward.y, forward.z };
	lefthandDirection = { lefthand.x, lefthand.y, lefthand.z };
}

void CameraClass::setRotation(const XMFLOAT3&& rot)
{
	rotation = rot;
	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.z, rotation.y, rotation.x);
	D3DXVECTOR3 forward = { 0.0f, 0.0f, -1.0f };
	D3DXVECTOR3 lefthand = { 1.0f, 0.0f, 0.0f };
	D3DXVec3TransformCoord(&forward, &forward, &rotationMatrix);
	D3DXVec3Normalize(&forward, &forward);
	D3DXVec3TransformCoord(&lefthand, &lefthand, &rotationMatrix);
	D3DXVec3Normalize(&lefthand, &lefthand);
	forwardDirection = { forward.x, forward.y, forward.z };
	lefthandDirection = { lefthand.x, lefthand.y, lefthand.z };
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
	m_matriceData.projection = XMMatrixPerspectiveFovLH(fov, ASPECTRATIO, near_far.x, near_far.y);

	decltype(m_matriceData) shaderMatricesData = m_matriceData;
	shaderMatricesData.world = XMMatrixTranspose(shaderMatricesData.world);
	shaderMatricesData.view = XMMatrixTranspose(shaderMatricesData.view);
	shaderMatricesData.projection = XMMatrixTranspose(shaderMatricesData.projection);
	m_matrices.UpdateData(&shaderMatricesData);
	m_matrices.UpdateGpu(device, context);
	m_matrices.BindVertexShader(device, context);

	m_frustum->ConstructFrustum(50.0f, m_matriceData.projection, m_matriceData.view);
}

void CameraClass::moveLeft(float step)
{
	position.x += lefthandDirection.x * step;
	position.y += lefthandDirection.y * step;
	position.z += lefthandDirection.z * step;
}

void CameraClass::moveRight(float step)
{
	position.x -= lefthandDirection.x * step;
	position.y -= lefthandDirection.y * step;
	position.z -= lefthandDirection.z * step;
}

void CameraClass::moveForward(float step)
{
	position.x += forwardDirection.x * step;
	position.y += forwardDirection.y * step;
	position.z += forwardDirection.z * step;
}
void CameraClass::moveBackward(float step)
{
	position.x -= forwardDirection.x * step;
	position.y -= forwardDirection.y * step;
	position.z -= forwardDirection.z * step;
}

void CameraClass::turnLeft(float step)
{
	XMFLOAT3 rot = getRotation();
	rot.z -= step * 0.017453292f;
	setRotation(rot);
}
void CameraClass::turnRight(float step)
{
	XMFLOAT3 rot = getRotation();
	rot.z += step * 0.017453292f;
	setRotation(rot);
}
void CameraClass::lookUp(float step)
{
	XMFLOAT3 rot = getRotation();
	rot.y += step * 0.017453292f;
	setRotation(rot);
}
void CameraClass::lookDown(float step)
{
	XMFLOAT3 rot = getRotation();
	rot.y -= step * 0.017453292f;
	setRotation(rot);
}
void CameraClass::twistLeft(float step)
{
	XMFLOAT3 rot = getRotation();
	rot.x -= step * 0.017453292f;
	setRotation(rot);
}
void CameraClass::twistRight(float step)
{
	XMFLOAT3 rot = getRotation();
	rot.x += step * 0.017453292f;
	setRotation(rot);
}

FrustumClass* CameraClass::getFrustum()
{
	return m_frustum.get();
}