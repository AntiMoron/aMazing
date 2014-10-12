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
	vertex vertices[36];
	XMFLOAT3 pos[] = { 
		XMFLOAT3(-0.5f, -0.5f, 0.5f),
		XMFLOAT3(-0.5f, -0.5f, -0.5f),
		XMFLOAT3(0.5f, -0.5f, -0.5f),
		XMFLOAT3(0.5f, -0.5f, 0.5f),
		XMFLOAT3(-0.5f, 0.5f, 0.5f),
		XMFLOAT3(0.5f, 0.5f, 0.5f),
		XMFLOAT3(0.5f, 0.5f, -0.5f),
		XMFLOAT3(-0.5f, 0.5f, -0.5f) };
	XMFLOAT3 nor[] = {
		XMFLOAT3(0.0f, -1.0f, 0.0f),
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 1.0f),
		XMFLOAT3(1.0f, 0.0, 0.0f),
		XMFLOAT3(0.0f, 0.0, -1.0f),
		XMFLOAT3(-1.0f, 0.0f, 0.0f)
	};
	XMFLOAT2 tex[] = {
		XMFLOAT2(1.0f, 0.0f),
		XMFLOAT2(1.0f, 1.0f),
		XMFLOAT2(0.0f, 1.0f),
		XMFLOAT2(0.0f, 0.0f)
	};
	
	WORD vertexIndices[] =
	{
		1, 2, 3, 3, 4, 1,
		5, 6, 7, 7, 8, 5,
		1, 4, 6, 6, 5, 1,
		4, 3, 7, 7, 6, 4,
		3, 2, 8, 8, 7, 3,
		2, 1, 5, 5, 8, 2
	};
	WORD normalIndices[] = {
		1,1,1,1,1,1,
		2,2,2,2,2,2,
		3,3,3,3,3,3,
		4,4,4,4,4,4,
		5,5,5,5,5,5,
		6,6,6,6,6,6
	};
	WORD textureIndices[] =
	{
		1, 2, 3, 3, 4, 1,
		4, 1, 2, 2, 3, 4,
		4, 1, 2, 2, 3, 4,
		4, 1, 2, 2, 3, 4,
		4, 1, 2, 2, 3, 4,
		4, 1, 2, 2, 3, 4
	};
	for (int i = 0; i < 36; i++)
	{
		vertices[i].position = pos[vertexIndices[i] - 1];
		vertices[i].normal = nor[normalIndices[i] - 1];
		vertices[i].texture = tex[textureIndices[i] - 1];
	}
	hr = m_vertices.Initialize(device, context, vertices, 36);
	if (FAILED(hr))
	{
		return hr;
	}
//Initialize PRS & PRS buffer
	hr = BasicObject::Initialize(device, context);
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

void BlockClass::Shutdown()
{
	m_vertices.Shutdown();
}



void BlockClass::Render(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
//Update PRS data
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	SHADERS.bindPair("Basic3D" , device, context);
	BasicObject::UpdatePRS(device, context);
	m_vertices.Render(device,context);
}