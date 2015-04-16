#pragma once

namespace aMazing
{
	class aNoCopyable
	{
	public:
		//default constructor
		aNoCopyable(){}
		//remove its copy constructor
		aNoCopyable(const aNoCopyable&) = delete;
		//remove its operator =.
		aNoCopyable& operator = (const aNoCopyable&) = delete;
	};
}
