#pragma once

#include<D3D11.h>
#include<D3DX11.h>
#include<xnamath.h>
#include"vertex.hpp"
#include"InstanceObj.hpp"
#include"GPUConstantBuffer.hpp"
#include"GPUVerticesBuffer.hpp"

class BasicObject
{
public:
	BasicObject();
	~BasicObject();

	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context);

	void setPosition(const XMFLOAT3& val);
	void setPosition(const XMFLOAT3&& val);

	void setRotation(const XMFLOAT3& val);
	void setRotation(const XMFLOAT3&& val);

	void setScaling(const XMFLOAT3& val);
	void setScaling(const XMFLOAT3&& val);


	HRESULT UpdatePRS(ID3D11Device* device,
		ID3D11DeviceContext* context);

	XMFLOAT3 getPosition() const;
	XMFLOAT3 getRotation() const;
	XMFLOAT3 getScaling() const;

protected:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scaling;
	PRS	m_prsData;
	GPUVerticesBuffer<vertex> m_vertices;
	GPUConstantBuffer<PRS>	  m_prsBuffer;
};

