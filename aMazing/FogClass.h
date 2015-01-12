#pragma once
#include<memory>
#include"GPUConstantBuffer.hpp"
#include<xnamath.h>
#include"ShaderManager.h"

class FogClass
{
public:
	FogClass();
	~FogClass();
	
	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context);

	void setFogColor(XMFLOAT3 color);
	void setFogNearDist(float val_near);
	void setFogFarDist(float val_far);

	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context);
private:
	volatile bool is_inited;
	struct FOG_DESC
	{
		float nearDist;
		float farDist;
		XMFLOAT3 fogColor;
	}fogDesc;
	std::unique_ptr<GPUConstantBuffer<FOG_DESC> > fogBuffer;
};