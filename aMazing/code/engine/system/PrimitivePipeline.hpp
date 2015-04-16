#pragma once
#include<memory>
#include"D3DManager.hpp"
#include"BlockClass.hpp"
#include"RectangleObject.hpp"
#include"LineClass.hpp"
#include"../../common/CommonDef.hpp"
#include"../system/thread/aThreadSafeSingleton.hpp"

namespace aMazing
{
	class PrimitivePipeline : public aThreadSafeSingleton<PrimitivePipeline>
	{
	public:
//		static PrimitivePipeline& getInstance();

		HRESULT Initialize(ID3D11Device* device, 
			ID3D11DeviceContext* context);

		void RenderRectangle(unsigned short l,unsigned short t,
			unsigned short r, unsigned short b);
		void RenderBox(float x,float y,float z,
			float rx,float ry,float rz,
			float sx,float sy,float sz);
		void RenderLine(float sx, float sy, float sz,
			float ex, float ey, float ez);
	private:
		std::shared_ptr<ID3D11Device> devicePtr;
		std::shared_ptr<ID3D11DeviceContext> contextPtr;
		std::unique_ptr<LineClass> line;
		std::unique_ptr<BlockClass> blk;
		std::unique_ptr<RectangleObject> rec;
	};
}
#define GRAPHICS (aMazing::PrimitivePipeline::getInstance())
