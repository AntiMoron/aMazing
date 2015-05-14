#pragma once
#include"../../common/CommonDxSupport.hpp"
#include"EffectClass.hpp"
namespace aMazing
{
	class DepthMap : public EffectClass
	{
	public:
		template<typename ...T>
		void Render(ID3D11Device* device,
			ID3D11DeviceContext* context,
			std::function<void(ID3D11Device*, ID3D11DeviceContext*)> renderFunction)
		{
			clearRenderTarget(device, context);
			clearDepthStencil(device, context);
			setRenderTarget(device, context);
			SHADERS.bindPair("DepthMap", device, context);
			SHADERS.DisableShaderBind();
			renderFunction(device, context);
			SHADERS.EnableShaderBind();
		}
	};
}
