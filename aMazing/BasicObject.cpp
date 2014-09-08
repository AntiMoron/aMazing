#include "BasicObject.h"


BasicObject::BasicObject()
{
	m_prsData.reset(new PRS);
}


BasicObject::~BasicObject()
{
}

HRESULT BasicObject::Initialize(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	HRESULT hr;
	position = { .0f, .0f, .0f };
	rotation = { .0f, .0f, .0f };
	scaling = { 1.0f, 1.0f, 1.0f };

	m_prsData->position = XMMatrixTranslation(position.x, position.y, position.z);
	m_prsData->rotation = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	m_prsData->scaling = XMMatrixScaling(scaling.x, scaling.y, scaling.z);
	
	for (int i = 0;i<4;i++)
	{
		for (int j = 0; j < i; j++)
		{
			m_prsData->position.m[i][j] = m_prsData->position.m[j][i];
			m_prsData->rotation.m[i][j] = m_prsData->rotation.m[j][i];
			m_prsData->scaling.m[i][j] = m_prsData->scaling.m[j][i];
		}
	}

	hr = m_prsBuffer.Initialize(device, context, 1);	//PRS info is bind to vertex shader slot 1
	if (FAILED(hr))
	{
		return hr;
	}
	hr = m_prsBuffer.UpdateData(m_prsData.get());
	if (FAILED(hr))
	{
		return hr;
	}
	hr = m_prsBuffer.UpdateGpu(device, context);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = m_prsBuffer.BindVertexShader(device, context);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

void BasicObject::setPosition(const XMFLOAT3& val)
{
	position = val;
}

void BasicObject::setPosition(XMFLOAT3&& val)
{
	position = val;
}

void BasicObject::setRotation(const XMFLOAT3& val)
{
	rotation = val;
}

void BasicObject::setRotation(XMFLOAT3&& val)
{
	rotation = val;
}

void BasicObject::setScaling(const XMFLOAT3& val)
{
	scaling = val;
}

void BasicObject::setScaling(XMFLOAT3&& val)
{
	scaling = val;
}


HRESULT BasicObject::UpdatePRS(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	HRESULT hr;
	m_prsData->position = XMMatrixTranslation(position.x, position.y, position.z);
	m_prsData->rotation = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	m_prsData->scaling = XMMatrixScaling(scaling.x, scaling.y, scaling.z);

	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j < i; j++)
		{
			m_prsData->position.m[i][j] = m_prsData->position.m[j][i];
			m_prsData->rotation.m[i][j] = m_prsData->rotation.m[j][i];
			m_prsData->scaling.m[i][j] = m_prsData->scaling.m[j][i];
		}
	}

	hr = m_prsBuffer.UpdateData(m_prsData.get());
	if (FAILED(hr))
	{
		return hr; 
	}
	hr = m_prsBuffer.UpdateGpu(device, context);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = m_prsBuffer.BindVertexShader(device, context);
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

XMFLOAT3 BasicObject::getPosition()const
{
	return position;
}

XMFLOAT3 BasicObject::getRotation() const
{
	return rotation;
}

XMFLOAT3 BasicObject::getScaling() const
{
	return scaling;
}