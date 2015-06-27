#pragma once
#include<memory>
#include"../D3DManager.hpp"
#include"BlockClass.hpp"
#include"RectangleObject.hpp"
#include"LineClass.hpp"
#include"../../../common/CommonDef.hpp"
#include"../../system/thread/aThreadSafeSingleton.hpp"

namespace aMazing
{
	class PrimitivePipeline : public aThreadSafeSingleton<PrimitivePipeline>
	{
	private:
		friend class aThreadSafeSingleton<PrimitivePipeline>;
		PrimitivePipeline(){}
	public:
		HRESULT initialize()
		{
			HRESULT hr;
			blk = std::make_shared<BlockClass>();
			rec = std::make_shared<RectangleObject>();
			line = std::make_shared<LineClass>();
			ID3D11Device* device = D3DManager::getDevice(MANAGED_DEVICE_TYPE::DEFAULT_DEVICE);
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

		void RenderRectangle(unsigned short l,unsigned short t,
			unsigned short r, unsigned short b)
		{
			ID3D11DeviceContext* context = D3DManager::getContext(MANAGED_CONTEXT_TYPE::DEFAULT_CONTEXT);
			D3DManager::disableDepth(context);
			rec->render(context, l, t, r, b);
			D3DManager::enableDepth(context);
		}

		void RenderBox(float x,float y,float z,
			float rx,float ry,float rz,
			float sx,float sy,float sz)
		{
			blk->setPosition(XMFLOAT3(x, y, z));
			blk->setRotation(XMFLOAT3(rx, ry, rz));
			blk->setScaling(XMFLOAT3(sx, sy, sz));
			blk->render(D3DManager::getContext(MANAGED_CONTEXT_TYPE::DEFAULT_CONTEXT));
		}

		void RenderLine(float sx, float sy, float sz,
			float ex, float ey, float ez)
		{
			line->render(D3DManager::getContext(MANAGED_CONTEXT_TYPE::DEFAULT_CONTEXT),
				sx, sy, sz, ex, ey, ez);
		}
	private:
		std::shared_ptr<LineClass> line;
		std::shared_ptr<BlockClass> blk;
		std::shared_ptr<RectangleObject> rec;
	};
}
#define GRAPHICS (aMazing::PrimitivePipeline::getInstance())
