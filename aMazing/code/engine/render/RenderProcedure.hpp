#pragma once
#include"RenderStep.hpp"
namespace aMazing
{
	class RenderProcedure
	{
	public:
		RenderProcedure(){}
		~RenderProcedure(){}
	private:
		std::shared_ptr<RenderStep> steps;
	};
}
