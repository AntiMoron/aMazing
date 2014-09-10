#include "PrimitivePipeline.h"

PrimitivePipeline* PrimitivePipeline::instance = nullptr;

PrimitivePipeline::PrimitivePipeline()
{
}


PrimitivePipeline::~PrimitivePipeline()
{
}


PrimitivePipeline& PrimitivePipeline::getInstance()
{
	if (instance == nullptr)
	{
		instance = new PrimitivePipeline;
	}
	return *instance;
}

HRESULT PrimitivePipeline::Initialize(D3DClass* d3d)
{
	HRESULT hr;
	d3dptr = d3d;
	ID3D11Device* device = d3dptr->getDevice();
	ID3D11DeviceContext* context = d3dptr->getContext();
	blk.reset(new BlockClass);
	rec.reset(new RectangleClass);
	line.reset(new LineClass);
	hr = blk->Initialize(device,context);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = rec->Initialize(device, context);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = line->Initialize(device, context);
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK; 
}

HRESULT PrimitivePipeline::Shutdown()
{
	blk->Shutdown();
	rec->Shutdown();
	line->Shutdown();
	return S_OK;
}

void PrimitivePipeline::RenderRectangle(unsigned short l, unsigned short t,
	unsigned short r, unsigned short b)
{
	ID3D11Device* device = d3dptr->getDevice();
	ID3D11DeviceContext* context = d3dptr->getContext();
	d3dptr->DisableDepth();
	rec->Render(device,context,l,t,r,b);
	d3dptr->EnableDepth();
}

void PrimitivePipeline::RenderBox(float x, float y, float z,
	float rx, float ry, float rz,
	float sx, float sy, float sz)
{
	ID3D11Device* device = d3dptr->getDevice();
	ID3D11DeviceContext* context = d3dptr->getContext();
	blk->setPosition(XMFLOAT3(x, y, z));
	blk->setRotation(XMFLOAT3(rx, ry, rz));
	blk->setScaling(XMFLOAT3(sx, sy, sz));
	blk->Render(device, context);
}


void PrimitivePipeline::RenderLine(float sx, float sy, float sz,
	float ex, float ey, float ez)
{
	ID3D11Device* device = d3dptr->getDevice();
	ID3D11DeviceContext* context = d3dptr->getContext();
	line->Render(device, context, sx, sy, sz, ex, ey, ez);
}