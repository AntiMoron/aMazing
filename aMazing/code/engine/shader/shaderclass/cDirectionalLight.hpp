#pragma once

#include"cAmbientLight.hpp"

namespace aMazing
{
	namespace shaderObjects
	{
		struct cDirectionalLight : public cAmbientLight
		{
			XMFLOAT4 vLightDir;
		};
	}
}