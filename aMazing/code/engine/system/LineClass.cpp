#include "LineClass.hpp"
using namespace aMazing;
LineClass::LineClass()
{
	is_inited = false;
}


LineClass::~LineClass()
{
}

HRESULT LineClass::initialize(ID3D11Device* device)
{
	HRESULT hr;
	hr = m_color.initialize(device, 1);
	if (FAILED(hr))
	{
		return hr;
	}
	
	Vertex verts[] = { 
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(), XMFLOAT2(0.0f, 0.0f) }
	};
	hr = m_vertices.initialize(device, verts, 2);
	if (FAILED(hr))
	{	
		return hr;
	}
	ID3D11DeviceContext* context;
	device->GetImmediateContext(&context);
	setLineColor(context, 1.0f, 1.0f, 1.0f, 1.0f);
	context->Release();
	lastPosX[0] = 0.0f;
	lastPosY[0] = 0.0f;
	lastPosZ[0] = 0.0f;
	lastPosX[1] = 1.0f;
	lastPosY[1] = 1.0f;
	lastPosZ[1] = 1.0f;
	is_inited = true;
	return S_OK;
}

void LineClass::setLineColor(ID3D11DeviceContext* context, 
	float r, float g, float b, float a)
{
	if (!(r == this->r && (g == this->g) && (b == this->b) && (a == this->a)))
	{
		XMFLOAT4 color = { r, g, b, a };
		m_color.updateData(&color);
		m_color.updateGpu(context);
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
}


void LineClass::render(ID3D11DeviceContext* context,
	float sx, float sy, float sz,
	float ex, float ey, float ez)
{
	if (!(sx == lastPosX[0] && (sy == lastPosY[0]) && (sz == lastPosZ[0])
		&& (ex == lastPosX[1]) && (ex == lastPosY[1]) && (ex == lastPosZ[1])))
	{
		lastPosX[0] = sx;
		lastPosY[0] = sy;
		lastPosZ[0] = sz;
		lastPosX[1] = ex;
		lastPosY[1] = ey;
		lastPosZ[1] = ez;	
		Vertex verts[] = {
			{ XMFLOAT3(sx, sy, sz),XMFLOAT3(), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(ex, ey, ez), XMFLOAT3(), XMFLOAT2(0.0f, 0.0f) }
		};
		m_vertices.updateVertices(context, verts, 2);
	}
	SHADERS.bindPair("BasicLine", context);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_color.bindPixelShader(context);
	m_vertices.render(context);
}

bool LineClass::isInited() const
{
	return is_inited;
}
