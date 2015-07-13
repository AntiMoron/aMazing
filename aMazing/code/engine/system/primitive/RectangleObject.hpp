#pragma once
#include"../GlobalWindow.hpp"
#include"../../../common/CommonDef.hpp"
#include"../../shader/ShaderManager.hpp"
#include"../mesh/GPUMutableVerticesBuffer.hpp"
#include"../../../engine/data/Vertex.hpp"
#include"../../../common/CommonDxSupport.hpp"
namespace aMazing
{
	class RectangleObject
	{
	public:
		HRESULT initialize(ID3D11Device* device)
		{
			HRESULT hr;
			float left, right, top, bottom;
			left = (float)(RESWIDTH / -2.0f) + 0.0f;
			right = left + 100.0f;
			top = (float)(RESHEIGHT / 2.0f) + 0.0f;
			bottom = top - 50.0f;
			left = 2.0f * left / RESWIDTH;
			right = 2.0f * right / RESWIDTH;
			top = 2.0f * top / RESHEIGHT;
			bottom = 2.0f * bottom / RESHEIGHT;
			Vertex vertices[6];
			vertices[0].position.x = left;
			vertices[0].position.y = top;
			vertices[0].position.z = 0.0f;
			vertices[0].texture.x = 0.0f;
			vertices[0].texture.y = 0.0f;

			vertices[1].position.x = right;
			vertices[1].position.y = bottom;
			vertices[1].position.z = 0.0f;
			vertices[1].texture.x = 1.0f;
			vertices[1].texture.y = 1.0f;

			vertices[2].position.x = left;
			vertices[2].position.y = bottom;
			vertices[2].position.z = 0.0f;
			vertices[2].texture.x = 0.0f;
			vertices[2].texture.y = 1.0f;
			//第二个三角
			vertices[3].position.x = left;
			vertices[3].position.y = top;
			vertices[3].position.z = 0.0f;
			vertices[3].texture.x = 0.0f;
			vertices[3].texture.y = 0.0f;

			vertices[4].position.x = right;
			vertices[4].position.y = top;
			vertices[4].position.z = 0.0f;
			vertices[4].texture.x = 1.0f;
			vertices[4].texture.y = 0.0f;

			vertices[5].position.x = right;
			vertices[5].position.y = bottom;
			vertices[5].position.z = 0.0f;
			vertices[5].texture.x = 1.0f;
			vertices[5].texture.y = 1.0f;

			hr = verts.initialize(device, vertices, 6, nullptr, 0);
			if (FAILED(hr))
			{
				return hr;
			}
			return S_OK;
		}

		void render(ID3D11DeviceContext* context,
			int ileft,
			int itop,
			int iright,
			int ibottom)
		{
			SHADERS.push("Basic2D", context);
			float left, right, top, bottom;
			left = (float)(RESWIDTH / -2.0f) + ileft;
			right = left + (iright - ileft);
			top = (float)(RESHEIGHT / 2.0f) - itop;
			bottom = top - (ibottom - itop);
			left = 2.0f * left / RESWIDTH;
			right = 2.0f * right / RESWIDTH;
			top = 2.0f * top / RESHEIGHT;
			bottom = 2.0f * bottom / RESHEIGHT;

			Vertex vertices[6];
			vertices[0].position.x = left;
			vertices[0].position.y = top;
			vertices[0].position.z = 0.0f;
			vertices[0].texture.x = 0.0f;
			vertices[0].texture.y = 0.0f;

			vertices[1].position.x = right;
			vertices[1].position.y = bottom;
			vertices[1].position.z = 0.0f;
			vertices[1].texture.x = 1.0f;
			vertices[1].texture.y = 1.0f;

			vertices[2].position.x = left;
			vertices[2].position.y = bottom;
			vertices[2].position.z = 0.0f;
			vertices[2].texture.x = 0.0f;
			vertices[2].texture.y = 1.0f;
			//第二个三角
			vertices[3].position.x = left;
			vertices[3].position.y = top;
			vertices[3].position.z = 0.0f;
			vertices[3].texture.x = 0.0f;
			vertices[3].texture.y = 0.0f;

			vertices[4].position.x = right;
			vertices[4].position.y = top;
			vertices[4].position.z = 0.0f;
			vertices[4].texture.x = 1.0f;
			vertices[4].texture.y = 0.0f;

			vertices[5].position.x = right;
			vertices[5].position.y = bottom;
			vertices[5].position.z = 0.0f;
			vertices[5].texture.x = 1.0f;
			vertices[5].texture.y = 1.0f;
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			verts.updateVertices(context, vertices, 6);
			verts.render(context);
			SHADERS.pop(context);
		}

	private:
		GPUMutableVerticesBuffer<Vertex> verts;
	};
}
