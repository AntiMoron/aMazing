#pragma once
#include<D3D11.h>
#include<D3DX11.h>
#include"Vertex.hpp"
#include"GPUConstantBuffer.hpp"
#include"GPUMutableVerticesBuffer.hpp"
#include"ShaderManager.h"

class LineClass
{
public:
	LineClass();
	~LineClass();

	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context);

	void setLineColor(ID3D11Device* device,
		ID3D11DeviceContext* context, 
		float r, float g, float b, float a);

	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		float sx,float sy,float sz,
		float ex,float ey,float ez);

	void Shutdown();

	bool isInited()const;
private:
	bool is_inited;
	float r, g, b, a;
	float lastPosX[2],lastPosY[2],lastPosZ[2];
	GPUConstantBuffer<XMFLOAT4> m_color;
	GPUMutableVerticeBuffer<Vertex> m_vertices;
};

