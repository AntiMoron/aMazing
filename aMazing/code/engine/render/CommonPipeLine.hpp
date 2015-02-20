#pragma once

#include"RenderComponents.hpp"

namespace aMazing
{
	using namespace RenderTypeEnum;

	template<MATERIAL_TYPE T>
	class CommonPipeLine
	{
	public:
		template<MATERIAL_TYPE C>
		CommonPipeLine& then(ID3D11Device* device, ID3D11DeviceContext* context,
			RenderComponents<C>& renderPass)
		{
			renderPass.Render(device, context);
			return *this;
		}
	};

	template<RENDER_TYPE T>
	class CommonPipeLine
	{
	public:
		template<RENDER_TYPE C>
		CommonPipeLine& then(ID3D11Device* device,ID3D11DeviceContext* context,
			RenderComponents<C>& renderPass)
		{
			renderPass.Render(device, context);
			return *this;
		}
	};
}
