#include "Maze.h"


Maze::Maze()
{
	m_map = nullptr;
	width = -1;
	height = -1;
}


Maze::~Maze()
{
	if (m_map != nullptr)
	{
		delete [] m_map;
		m_map = nullptr;
	}
}

void Maze::Render(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	for (int i = 0; i<height ; i++)
	{
		for (int j = 0; j < width; j++)
		{
			XMFLOAT3 position = { i * 0.05f, 0.05f, j*0.05f };
			XMFLOAT3 rotation = { .0f, .0f, .0f };
			XMFLOAT3 scaling = { 0.05f, 0.05f, 0.05f };
			if (m_map[i * width + j] == true)
			{
				position = { i * 0.05f, 0.05f, j*0.05f };
			}
			else
			{
				position = { i * 0.05f, 0.0f, j*0.05f };
			}
			GRAPHICS.RenderBox(position.x, position.y, position.z, 
				rotation.x, rotation.y, rotation.z,
				scaling.x, scaling.y, scaling.z );
		}
	}
}