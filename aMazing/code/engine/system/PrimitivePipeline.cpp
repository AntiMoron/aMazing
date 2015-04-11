#include "PrimitivePipeline.hpp"

using namespace aMazing;

PrimitivePipeline& PrimitivePipeline::getInstance()
{
	static PrimitivePipeline instance;
	return instance;
}

HRESULT PrimitivePipeline::Initialize(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	HRESULT hr;
	devicePtr.reset(device);
	contextPtr.reset(context);
	blk = std::make_unique<BlockClass>();
	rec = std::make_unique<RectangleObject>();
	line = std::make_unique<LineClass>();
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

void PrimitivePipeline::RenderRectangle(unsigned short l, unsigned short t,
	unsigned short r, unsigned short b)
{
	D3DManager::disableDepth(contextPtr.get());
	rec->Render(devicePtr.get(),contextPtr.get(),l,t,r,b);
	D3DManager::enableDepth(contextPtr.get());
}

void PrimitivePipeline::RenderBox(float x, float y, float z,
	float rx, float ry, float rz,
	float sx, float sy, float sz)
{
	blk->setPosition(XMFLOAT3(x, y, z));
	blk->setRotation(XMFLOAT3(rx, ry, rz));
	blk->setScaling(XMFLOAT3(sx, sy, sz));
	blk->Render(devicePtr.get(), contextPtr.get());
}


void PrimitivePipeline::RenderLine(float sx, float sy, float sz,
	float ex, float ey, float ez)
{
	line->Render(devicePtr.get(), contextPtr.get(), sx, sy, sz, ex, ey, ez);
}
