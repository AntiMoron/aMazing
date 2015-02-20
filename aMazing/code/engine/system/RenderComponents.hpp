#pragma once
#include"../../common/CommonDxSupport.hpp"
#include"MaterialType.hpp"
#include"ShaderManager.h"

using namespace RenderTypeEnum;

template<MATERIAL_TYPE T>
class RenderComponents
{
public:
	void setArguments(...);
};

template<>
class RenderComponents<MATERIAL_PLASTIC>
{
public:
	void setArguments()
	{
		;
	}
private:
	;
};

