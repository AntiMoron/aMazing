#include "RectangleClass.hpp"
using namespace aMazing;

RectangleClass::RectangleClass()
{
}


RectangleClass::~RectangleClass()
{
}

HRESULT RectangleClass::Initialize(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	HRESULT hr;
	float left, right, top, bottom;
	left = (float)(WINWIDTH / -2.0f) + 0.0f;
	right = left + 100.0f;
	top = (float)(WINHEIGHT / 2.0f) + 0.0f;
	bottom = top - 50.0f;
	left = 2.0 * left / WINWIDTH;
	right = 2.0 * right / WINWIDTH;
	top = 2.0 * top / WINHEIGHT;
	bottom = 2.0 * bottom / WINHEIGHT;
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

	hr = verts.Initialize(device,context,vertices,6,nullptr,0);
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

void RectangleClass::Render(ID3D11Device* device,
	ID3D11DeviceContext* context,
	unsigned short ileft,
	unsigned short itop,
	unsigned short iright,
	unsigned short ibottom)
{
	float left, right, top, bottom;
	left = (float)(WINWIDTH / -2.0f) + ileft;
	right = left + (iright - ileft);
	top = (float)(WINHEIGHT / 2.0f) - itop;
	bottom = top - (ibottom - itop);
	left = 2.0 * left / WINWIDTH;
	right = 2.0 * right / WINWIDTH;
	top = 2.0 * top / WINHEIGHT;
	bottom = 2.0 * bottom / WINHEIGHT;

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
	verts.UpdateVertices(device,context,vertices,6);
	verts.Render(device, context);
}