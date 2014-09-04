#include "BlockClass.h"


BlockClass::BlockClass()
{
}


BlockClass::~BlockClass()
{
}

HRESULT BlockClass::Initialize(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	HRESULT hr = E_FAIL;
	vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	};
	WORD indices[] =
	{
		3, 1, 0,
		2, 1, 3,

		6, 4, 5,
		7, 4, 6,

		11, 9, 8,
		10, 9, 11,

		14, 12, 13,
		15, 12, 14,

		19, 17, 16,
		18, 17, 19,

		22, 20, 21,
		23, 20, 22
	};
	hr = m_vertices.Initialize(device, context, vertices, 24, indices, 36);
	if (FAILED(hr))
	{
		return hr;
	}
//Initialize PRS & PRS buffer

	position = {.0f,.0f,.0f};
	rotation = {.0f,.0f,.0f};
	scaling = {1.0f,1.0f,1.0f};
	m_prsData.position = XMMatrixTranslation(position.x, position.y, position.z);
	m_prsData.rotation = XMMatrixRotationRollPitchYaw(rotation.x ,rotation.y,rotation.z);
	m_prsData.scaling = XMMatrixScaling(scaling.x, scaling.y, scaling.z);

	m_prsData.position = XMMatrixTranspose(m_prsData.position);
	m_prsData.rotation = XMMatrixTranspose(m_prsData.rotation);
	m_prsData.scaling = XMMatrixTranspose(m_prsData.scaling);

	m_prsBuffer.Initialize(device, context,1);	//PRS info is bind to vertex shader slot 1
	m_prsBuffer.UpdateData(&m_prsData);
	m_prsBuffer.UpdateGpu(device, context);
	m_prsBuffer.BindVertexShader(device, context);

	return S_OK;
}

void BlockClass::Shutdown()
{
	m_vertices.Shutdown();
}


void BlockClass::setPosition(const XMFLOAT3& val)
{
	position = val;
}

void BlockClass::setPosition(const XMFLOAT3&& val)
{
	position = val;
}

void BlockClass::setRotation(const XMFLOAT3& val)
{
	rotation = val;
}

void BlockClass::setRotation(const XMFLOAT3&& val)
{
	rotation = val;
}

void BlockClass::setScaling(const XMFLOAT3& val)
{
	scaling = val;
}

void BlockClass::setScaling(const XMFLOAT3&& val)
{
	scaling = val;
}

XMFLOAT3 BlockClass::getPosition()const 
{
	return position;
}

XMFLOAT3 BlockClass::getRotation() const
{
	return rotation;
}

XMFLOAT3 BlockClass::getScaling() const
{
	return scaling;
}

void BlockClass::Render(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
//Update PRS data
	m_prsData.position = XMMatrixTranslation(position.x, position.y, position.z);
	m_prsData.rotation = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	m_prsData.scaling = XMMatrixScaling(scaling.x, scaling.y, scaling.z);
	
	m_prsData.position = XMMatrixTranspose(m_prsData.position);
	m_prsData.rotation = XMMatrixTranspose(m_prsData.rotation);
	m_prsData.scaling = XMMatrixTranspose(m_prsData.scaling);

	m_prsBuffer.UpdateData(&m_prsData);
	m_prsBuffer.UpdateGpu(device, context);
	m_prsBuffer.BindVertexShader(device, context);

	m_vertices.Render(device,context);
}