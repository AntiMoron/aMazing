#pragma once

#include"BlockClass.h"

class Maze
{
public:
	Maze();
	~Maze();

	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		BlockClass* block);
private:
	friend class MazeGenerator;
	bool* m_map;
	int width, height;
};