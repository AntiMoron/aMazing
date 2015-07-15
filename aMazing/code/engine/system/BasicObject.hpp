#pragma once
#include<memory>
#include"../data/Vertex.hpp"
#include"InstanceObj.hpp"
#include"GPUConstantBuffer.hpp"
#include"../../common/CommonDef.hpp" 

class BasicObject
{
public:
	BasicObject();
	virtual ~BasicObject()
	{
		;
	}

	HRESULT initialize(ID3D11Device* device);

	void setPosition(const XMFLOAT3& val);
	void setPosition(XMFLOAT3&& val);

	void setRotation(const XMFLOAT3& val);
	void setRotation(XMFLOAT3&& val);

	void setScaling(const XMFLOAT3& val);
	void setScaling(XMFLOAT3&& val);

	HRESULT updatePRS(ID3D11DeviceContext* context);

	XMFLOAT3 getPosition() const;
	XMFLOAT3 getRotation() const;
	XMFLOAT3 getScaling() const;

protected:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scaling;
	std::unique_ptr<PRS> m_prsData;
	static GPUConstantBuffer<PRSShaderData> m_prsBuffer;
};
