#pragma once
#include"MutableString.h"
#include"ShaderCompilerClass.h"

class ShaderClass
{
public:
	enum SHADER_TYPE
	{
		SHADER_NO_TYPE,
		SHADER_VERTEX,
		SHADER_PIXEL,
	};
	
	ShaderClass();
	~ShaderClass();
	
	SHADER_TYPE getType() const;
private:
	friend class VertexShaderClass;
	friend class PixelShaderClass;
	bool isInited;
	SHADER_TYPE type;
};

