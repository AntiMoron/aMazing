#pragma once
#include"../system/MutableString.hpp"
#include"../../common/CommonDef.hpp"
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
	
		ShaderClass()
		{
			isInited = false;
			type = SHADER_NO_TYPE;
			pShaderContextBuffer = nullptr;
			pClassLinkage = nullptr;
			pShaderReflector = nullptr;
		}
		virtual ~ShaderClass()
		{
			SAFE_RELEASE(pShaderContextBuffer);
			SAFE_RELEASE(pClassLinkage);
			SAFE_RELEASE(pShaderReflector);
		}
	
		const SHADER_TYPE getType() const
		{
			return type;
		}
	protected:
		volatile bool isInited;
		SHADER_TYPE type;
		ID3DBlob* pShaderContextBuffer;
		ID3D11ClassLinkage* pClassLinkage;
		ID3D11ShaderReflection* pShaderReflector;
	};
}
