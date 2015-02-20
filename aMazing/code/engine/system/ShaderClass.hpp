#pragma once
#include"MutableString.hpp"
#include"ShaderCompilerClass.hpp"
namespace aMazing
{
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
}
