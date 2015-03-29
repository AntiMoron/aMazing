#pragma once

#include<D3D11.h>
#include<D3DX11.h>
#include<D3DX10math.h>
#include<xnamath.h>
#include"WindowClass.hpp"
#include"GPUMutableVerticesBuffer.hpp"
#include"../../engine/data/Vertex.hpp"
#include"../../common/CommonDef.hpp"
namespace aMazing
{
	class RectangleClass
	{
	public:
		RectangleClass();
		~RectangleClass();

		HRESULT Initialize(ID3D11Device* device,
			ID3D11DeviceContext* context);

		void Render(ID3D11Device* device,
			ID3D11DeviceContext* context,
			unsigned short left,
			unsigned short top,
			unsigned short right,
			unsigned short bottom);
	private:
		GPUMutableVerticeBuffer<Vertex> verts;
	};
}
