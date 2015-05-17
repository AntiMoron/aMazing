#include "PrimitivePipeline.hpp"

using namespace aMazing;

HRESULT PrimitivePipeline::initialize(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	HRESULT hr;
	devicePtr.reset(device);
	contextPtr.reset(context);
	blk = std::make_shared<BlockClass>();
	rec = std::make_shared<RectangleObject>();
	line = std::make_shared<LineClass>();
	device->AddRef();
	context->AddRef();
	hr = blk->initialize(device);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = rec->initialize(device);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = line->initialize(device);
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
	rec->Render(contextPtr.get(),l,t,r,b);
	D3DManager::enableDepth(contextPtr.get());
}

void PrimitivePipeline::RenderBox(float x, float y, float z,
	float rx, float ry, float rz,
	float sx, float sy, float sz)
{
	blk->setPosition(XMFLOAT3(x, y, z));
	blk->setRotation(XMFLOAT3(rx, ry, rz));
	blk->setScaling(XMFLOAT3(sx, sy, sz));
	blk->render(contextPtr.get());
}


void PrimitivePipeline::RenderLine(float sx, float sy, float sz,
	float ex, float ey, float ez)
{
	line->render(contextPtr.get(), sx, sy, sz, ex, ey, ez);
}
