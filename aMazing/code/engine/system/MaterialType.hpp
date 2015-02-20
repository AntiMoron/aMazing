#pragma once

namespace aMazing
{
	namespace RenderTypeEnum{

		enum RENDER_TYPE
		{
			//render things as glow effect
			RENDER_GLOW,
			//reder things as blur effect
			RENDER_BLUR,
			//render things as light effect
			RENDER_LIGHT,
			//render things as fog effect
			RENDER_FOG,

			//the amount of valid render types.
			RENDER_TYPE_COUNT
		};

		enum MATERIAL_TYPE
		{
			//material from the void
			MATERIAL_VOID,
			//material of skins
			MATERIAL_SKIN,
			//material of a stone
			MATERIAL_STONE,
			//material of a fire
			MATERIAL_FIRE,
			//material of medal thing
			MATERIAL_MEDAL,
			//material of plastic
			MATERIAL_PLASTIC,
			//material of glass
			MATERIAL_GLASS,
			//material of a mirror
			MATERIAL_MIRROR,
			//material of deep water
			MATERIAL_DEEP_WATER,
			//material of liquid
			MATERIAL_LIQUID_WATER,
			//material of stream water
			MATERIAL_STREAM_WATER,

			//the amount of valid material type
			MATERIAL_TYPE_COUNT
		};
	}
}