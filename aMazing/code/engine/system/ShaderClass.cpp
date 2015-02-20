#include "ShaderClass.hpp"
using namespace aMazing;

ShaderClass::ShaderClass()
{
	isInited = false;
	type = SHADER_NO_TYPE;
}


ShaderClass::~ShaderClass()
{
	;
}


ShaderClass::SHADER_TYPE ShaderClass::getType() const
{
	return type;
}