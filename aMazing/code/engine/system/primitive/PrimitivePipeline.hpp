#pragma once
#include<memory>
#include<cstring>
#include"../D3DManager.hpp"
#include"BlockClass.hpp"
#include"RectangleObject.hpp"
#include"LineClass.hpp"
#include"../../../common/CommonDef.hpp"
#include"../../system/thread/aThreadSafeSingleton.hpp"
#include"../../font/font.hpp"

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

		void renderRectangle(int l,int t,
			int r, int b) const aNOEXCEPT
		{
			ID3D11DeviceContext* context = D3DManager::getContext(MANAGED_CONTEXT_TYPE::DEFAULT_CONTEXT);
			D3DManager::disableDepth(context);
			rec->render(context, l, t, r, b);
			D3DManager::enableDepth(context);
		}

		void renderBox(float x,float y,float z,
			float rx,float ry,float rz,
			float sx, float sy, float sz) const aNOEXCEPT
		{
			blk->setPosition(XMFLOAT3(x, y, z));
			blk->setRotation(XMFLOAT3(rx, ry, rz));
			blk->setScaling(XMFLOAT3(sx, sy, sz));
			blk->render(D3DManager::getContext(MANAGED_CONTEXT_TYPE::DEFAULT_CONTEXT));
		}

		void renderLine(float sx, float sy, float sz,
			float ex, float ey, float ez) const aNOEXCEPT
		{
			line->render(D3DManager::getContext(MANAGED_CONTEXT_TYPE::DEFAULT_CONTEXT),
				sx, sy, sz, ex, ey, ez);
		}

		void renderText(
			int x, int y,
			const wchar_t* text,
			size_t size = 16,
			const char* font = "./default.ttf",
			int resX = 72, int resY = 72) const aNOEXCEPT
		{
			ID3D11DeviceContext* context = D3DManager::getContext(MANAGED_CONTEXT_TYPE::DEFAULT_CONTEXT);
			int penX = x, penY = y;
			size_t cur = 0;
			while (text[cur] != L'\0')
			{
				if (penX > RESWIDTH || penY > RESHEIGHT)
				{
					return;
				}
				const FontTexture* bitmap = FONT.getFontBitmap(text[cur], size, font, resX, resY);
				if (!aMazing::isBlank(text[cur]))
				{
					bitmap->bindPS(D3DManager::getContext(MANAGED_CONTEXT_TYPE::DEFAULT_CONTEXT),
						0);
					int l = penX + bitmap->getBitmapLeft();
					int t = penY + bitmap->getBitmapTop();
					int r = l + bitmap->getWidth();
					int b = t + bitmap->getHeight();
					D3DManager::disableDepth(context);
					rec->render(context, l, t, r, b);
					D3DManager::enableDepth(context);
				}
				if (text[cur] != '\n')
				{
					penX += bitmap->getNextPenX();
					penY += bitmap->getNextPenY();
				}
				else
				{
					penX = 0;
					penY += bitmap->getOutlineY();
				}
				++cur;
			}
		}
	private:
		std::shared_ptr<LineClass> line;
		std::shared_ptr<BlockClass> blk;
		std::shared_ptr<RectangleObject> rec;
	};
}
#define GRAPHICS (aMazing::PrimitivePipeline::getInstance())
