#pragma once
#include"EffectClass.hpp"

namespace aMazing
{
	//Project the texture bound to slot 1 to scene
	class ProjectTexture :public EffectClass
	{
	public:
		template<typename ...T>
		void Render(ID3D11Device* device,
			ID3D11DeviceContext* context,
			common_tool::functionType<std::function<void(ID3D11Device*, ID3D11DeviceContext*)> > renderFunction)
		{
			setRenderTarget(device, context);
			clearRenderTarget(device, context);
			SHADERS.bindPair("ProjectionTex", device, context);
			SHADERS.DisableShaderBind();
			renderFunction(device, context);
			SHADERS.EnableShaderBind();
		}
	};
}
