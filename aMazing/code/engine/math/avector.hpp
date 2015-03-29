#pragma once

namespace aMazing
{
	struct aVec2
	{
		float x;
		float y;
		void beOne() throw()
		{
			x = 1.0f;
			y = 1.0f;
			z = 1.0f;
		}
		void beZero() throw()
		{
			x = 0;
			y = 0;
			z = 0;
		}
	};

	struct aVec3
	{
		float x;
		float y;
		float z;
		void beOne()
		{
			x = 1.0f;
			y = 1.0f;
			z = 1.0f;
		}
		void beZero()
		{
			x = 0;
			y = 0;
			z = 0;
		}
	};

	struct aVec4
	{
		float x;
		float y;
		float z;
		float w;
	};
}
