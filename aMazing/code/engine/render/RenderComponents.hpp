#pragma once
#include"../../common/CommonDxSupport.hpp"
#include"../system/MaterialType.hpp"
#include"../system/ShaderManager.hpp"

namespace aMazing
{
	using namespace RenderTypeEnum;

	template<MATERIAL_TYPE T>
	class RenderComponents
	{
	public:
		void setArguments(...);
		/*
		*@brief :
		*Do the rendering job for some specific material type.
		*/
		void Render(ID3D11Device* device, ID3D11DeviceContext* context);
	};

	template<RENDER_TYPE T>
	class RenderComponents
	{
	public:
		void setArguments(...);
		/*
		*@brief 
		*Do the rendering job for some specific render type.
		*/
		void Render(ID3D11Device* device, ID3D11DeviceContext* content);
	};
}
