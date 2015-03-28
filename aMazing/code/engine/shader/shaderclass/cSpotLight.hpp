#pragma once

#include"cAmbientLight.hpp"

namespace aMazing
{
	namespace shaderObjects
	{
		class cSpotLight : public cAmbientLight
		{
			XMFLOAT3 vLightPosition;
			XMFLOAT3 vLightDir;
		};
	}
}