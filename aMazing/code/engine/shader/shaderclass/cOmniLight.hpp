#pragma once

#include"cAmbientLight.hpp"
namespace aMazing
{
	namespace shaderObjects
	{
		struct cOmniLight : public cAmbientLight
		{
			XMFLOAT3 vLightPosition;
			float radius;
		};
	}
}
