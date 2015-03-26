#pragma once
#include"../exception/FailureException.hpp"
#include"ShaderVariable.hpp"
namespace aMazing
{
	class ShaderClassInstance
	{
	public:
		explicit ShaderClassInstance() = delete;
		//parameter
		explicit ShaderClassInstance(ID3D11ShaderReflection* pShaderReflector, const char* varName) throw(FailureException)
		{
			HRESULT hr = E_FAIL;
			if (!pShaderReflector)
			{
				throw FailureException("Reflector hasn't been inited.");
			}
			classVar = nullptr;
			classVar = pShaderReflector->GetVariableByName(varName);
			if (!classVar)
			{
				throw FailureException("ShaderClassInstance initialization failed.");
			}
		}

		ShaderVariable getMemberByName(const char* memberName)
		{
			//For Return Value Optimization
			return ShaderVariable(classVar->GetBuffer()->GetVariableByName(memberName));
		}

		ShaderVariable getMemberByIndex(unsigned int index)
		{
			return ShaderVariable(classVar->GetBuffer()->GetVariableByIndex(index));
		}
	private:
		ID3D11ShaderReflectionVariable* classVar;
	};
}