#pragma once
#include"ShaderManager.h"
#include"BasicObject.h"

class BlockClass : public BasicObject
{
public:
	BlockClass();
	~BlockClass();
	
	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context);

	void Render(ID3D11Device* device, 
		ID3D11DeviceContext* context);
};