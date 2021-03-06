#include "BasicObject.hpp"
using namespace aMazing;
GPUConstantBuffer<PRSShaderData> BasicObject::m_prsBuffer;

BasicObject::BasicObject()
{
	m_prsData.reset(new PRS);
}

HRESULT BasicObject::initialize(ID3D11Device* device)
{
	HRESULT hr;
	position = { .0f, .0f, .0f };
	rotation = { .0f, .0f, .0f };
	scaling = { 1.0f, 1.0f, 1.0f };

	PRSShaderData prsData;
	prsData.position = XMMatrixTranslation(position.x, position.y, position.z);
	prsData.rotation = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	prsData.scaling = XMMatrixScaling(scaling.x, scaling.y, scaling.z);
	
	prsData.position = XMMatrixTranspose(prsData.position);
	prsData.rotation = XMMatrixTranspose(prsData.rotation);
	prsData.scaling = XMMatrixTranspose(prsData.scaling);

	//store the data
	XMStoreFloat4x4(&(m_prsData->position), prsData.position);
	XMStoreFloat4x4(&(m_prsData->rotation), prsData.rotation);
	XMStoreFloat4x4(&(m_prsData->scaling), prsData.scaling);

	hr = m_prsBuffer.initialize(device, 1);	//PRS info is bind to Vertex shader slot 1
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


HRESULT BasicObject::updatePRS(ID3D11DeviceContext* context)
{
	HRESULT hr;
	PRSShaderData prsData;
	prsData.position = XMMatrixTranslation(position.x, position.y, position.z);
	prsData.rotation = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	prsData.scaling = XMMatrixScaling(scaling.x, scaling.y, scaling.z);

	prsData.position = XMMatrixTranspose(prsData.position);
	prsData.rotation = XMMatrixTranspose(prsData.rotation);
	prsData.scaling = XMMatrixTranspose(prsData.scaling);

	//store the data
	XMStoreFloat4x4(&(m_prsData->position), prsData.position);
	XMStoreFloat4x4(&(m_prsData->rotation), prsData.rotation);
	XMStoreFloat4x4(&(m_prsData->scaling), prsData.scaling);


	hr = m_prsBuffer.updateData(&prsData);
	if (FAILED(hr))
	{
		return hr; 
	}
	hr = m_prsBuffer.updateGpu(context);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = m_prsBuffer.BindVertexShader(context);
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
