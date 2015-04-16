#pragma once

namespace aMazing
{
	class aNoDefaultConstructor
	{
	public:
		//remove its default constructor.
		aNoDefaultConstructor() = delete;
	};
}
