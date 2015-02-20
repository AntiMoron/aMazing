#pragma once

#include"FunctionType.hpp"

class ScopeGuard
{
public:
	ScopeGuard(common_tool::functionType<std::function<void(void)> > o)
	{
		outer = o;
	}

	~ScopeGuard()
	{
		outer();
	}

	static ScopeGuard* makeScopeGuard(common_tool::functionType<std::function<void(void)> > o)
	{
		ScopeGuard* result = new ScopeGuard(o);
		return result;
	}
	std::function<void(void)> outer;
};
