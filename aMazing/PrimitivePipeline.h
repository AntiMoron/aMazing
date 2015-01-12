#pragma once
#include<memory>
#include"D3DClass.h"
#include"BlockClass.h"
#include"RectangleClass.h"
#include"Defines.hpp"
#include"LineClass.h"

class PrimitivePipeline
{
public:
	static PrimitivePipeline& getInstance();

	HRESULT Initialize(D3DClass* d3d);

	void RenderRectangle(unsigned short l,unsigned short t,
		unsigned short r, unsigned short b);
	void RenderBox(float x,float y,float z,
		float rx,float ry,float rz,
		float sx,float sy,float sz);
	void RenderLine(float sx, float sy, float sz,
		float ex, float ey, float ez);
private:
	PrimitivePipeline();
	~PrimitivePipeline();

	D3DClass* d3dptr;
	std::unique_ptr<LineClass> line;
	std::unique_ptr<BlockClass> blk;
	std::unique_ptr<RectangleClass> rec;
};

#define GRAPHICS (PrimitivePipeline::getInstance())