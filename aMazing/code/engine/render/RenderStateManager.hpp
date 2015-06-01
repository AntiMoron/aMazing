#pragma once
#include"../system/thread/aThreadSafeSingleton.hpp"

namespace aMazing
{
	enum RENDER_MODE
	{
		RENDER_WIREFRAME,
		RENDER_TRIANGLE_STRIP,
		RENDER_MODE_COUNT,
	};

	class RenderStateManager :public aThreadSafeSingleton<RenderStateManager>
	{
	private:
		friend class aThreadSafeSingleton<RenderStateManager>;
		volatile bool bIsWireframe;
		RenderStateManager()
		{
			bIsWireframe = false;
		}
	public:
		bool isWireframe() const volatile
		{
			return bIsWireframe;
		}
		void setWireframe(bool state) volatile
		{
			bIsWireframe = state;
		}
	};
}

#define RENDER_STATE_MANAGER (aMazing::RenderStateManager::getInstance())