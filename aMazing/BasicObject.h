#pragma once

#include<D3D11.h>
#include<D3DX11.h>
#include<xnamath.h>
#include"Vertex.hpp"
#include"InstanceObj.hpp"
#include"GPUConstantBuffer.hpp"
#include"GPUVerticesBuffer.hpp"
#include<memory>
#include"Defines.hpp" 

class BasicObject
{
public:
	BasicObject();
	~BasicObject();

	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context);
	void Shutdown();

	void setPosition(const XMFLOAT3& val);
	void setPosition(XMFLOAT3&& val);

	void setRotation(const XMFLOAT3& val);
	void setRotation(XMFLOAT3&& val);

	void setScaling(const XMFLOAT3& val);
	void setScaling(XMFLOAT3&& val);

	HRESULT UpdatePRS(ID3D11Device* device,
		ID3D11DeviceContext* context);

	XMFLOAT3 getPosition() const;
	XMFLOAT3 getRotation() const;
	XMFLOAT3 getScaling() const;

protected:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scaling;
	std::unique_ptr<PRS> m_prsData;
	GPUVerticesBuffer<Vertex> m_vertices;
	GPUConstantBuffer<PRSShaderData> m_prsBuffer;
};

