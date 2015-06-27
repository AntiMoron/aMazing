#pragma once
#include"../../common/CommonDef.hpp"
#include"FrameSemantic.hpp"
#include<vector>
#include<memory>

namespace aMazing
{
	class RenderStep
	{
	public:
		RenderStep()
		{
			previous = nullptr;
			next = nullptr;
		}
		~RenderStep()
		{
			aSAFE_DELETE(previous);
			aSAFE_DELETE(next);
		}

		RenderStep& then(RenderStep* pRenderStep)
		{
			next = pRenderStep;
			next->previous = this;
			return *next;
		}
		
		SemanticFrame* search(FRAME_SEMANTIC sem) const
		{
			for (const auto& semanticFrame : resourceViews)
			{
				if (semanticFrame->getSemantic() == sem)
				{
					return semanticFrame.get();
				}
			}
			return nullptr;
		}
	private:
		RenderStep* previous;
		RenderStep* next;
		std::vector<std::shared_ptr<SemanticFrame> > resourceViews;
	};
}
