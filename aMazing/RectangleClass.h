#pragma once

#include<D3D11.h>
#include<D3DX11.h>
#include<D3DX10math.h>
#include<xnamath.h>
#include"vertex.hpp"
#include"WindowClass.h"
#include"GPUMutableVerticesBuffer.hpp"
#include"Defines.hpp"

class RectangleClass
{
public:
	RectangleClass();
	~RectangleClass();

	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context);
	HRESULT Shutdown();

	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		unsigned short left,
		unsigned short top,
		unsigned short right,
		unsigned short bottom);
private:
	GPUMutableVerticeBuffer<vertex> verts;
};

