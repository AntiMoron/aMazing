#pragma once
#include"D3DClass.h"
#include"BlockClass.h"
#include"RectangleClass.h"
#include<memory>

class PrimitivePipeline
{
public:
	static PrimitivePipeline& getInstance();

	HRESULT Initialize(D3DClass* d3d);
	HRESULT Shutdown();

	void RenderRectangle(unsigned short l,unsigned short t,
		unsigned short r, unsigned short b);
	void RenderBox(float x,float y,float z,
		float rx,float ry,float rz,
		float sx,float sy,float sz);
private:
	PrimitivePipeline();
	~PrimitivePipeline();

	static PrimitivePipeline* instance;
	D3DClass* d3dptr;
	std::unique_ptr<BlockClass> blk;
	std::unique_ptr<RectangleClass> rec;
};

#define GRAPHICS (PrimitivePipeline::getInstance())