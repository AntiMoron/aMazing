#pragma once
#include"../../common/CommonDxSupport.hpp"
#include"../data/Vertex.hpp"
#include"InstanceObj.hpp"
#include"GPUConstantBuffer.hpp"
#include"GPUVerticesBuffer.hpp"
#include<memory>
#include"../../common/CommonDef.hpp" 

class BasicObject
{
public:
	BasicObject();
	virtual ~BasicObject();

	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context);

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
	GPUConstantBuffer<PRSShaderData> m_prsBuffer;
};
