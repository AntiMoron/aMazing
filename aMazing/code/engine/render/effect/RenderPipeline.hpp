#pragma once

class RenderPipeline
{
public:
	static RenderPipeline& getInstance()
	{
		static RenderPipeline instance;
		return instance;
	}
	~RenderPipeline(){}
private:
	RenderPipeline(){}
};
