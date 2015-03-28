#pragma once

#include"cAmbientLight.hpp"

namespace aMazing
{
	namespace shaderObjects
	{
		struct cHemiAmbientLight : public cAmbientLight
		{
			XMFLOAT4 groundColor;
			XMFLOAT4 vDirUp;
		};
	}
}