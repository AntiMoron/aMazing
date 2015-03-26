#pragma once

#include"../../common/CommonDxSupport.hpp"
#include<memory>

namespace aMazing
{
	class ShaderVariable
	{
	public:
		explicit ShaderVariable() = delete;
		explicit ShaderVariable(ID3D11ShaderReflectionVariable* pVar)
		{
			pSharedVar.reset(pVar);
		}
		
		void setInt()
		{
		}
	private:
		std::shared_ptr<ID3D11ShaderReflectionVariable> pSharedVar;
	};
}