#pragma once
#include"PrimitivePipeline.h"

class Maze
{
public:
	Maze();
	~Maze();

	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context);
private:
	friend class MazeGenerator;
	bool* m_map;
	int width, height;
};