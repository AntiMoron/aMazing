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
	int halfy = height / 2;
	int halfx = width / 2;
	for (int i = -halfy; i< (height-halfy) ; i++)
	{
		for (int j = -halfx; j < (width - halfx); j++)
		{
			XMFLOAT3 position = { i * 0.005f, 0.005f, j*0.005f };
			XMFLOAT3 rotation = { .0f, .0f, .0f };
			XMFLOAT3 scaling = { 0.005f, 0.005f, 0.005f };
			if (m_map[(i+halfy) * width + (j+halfx)] == true)
			{
				position = { i * 0.005f, 0.005f, j*0.005f };
			}
			else
			{
				position = { i * 0.005f, 0.0f, j*0.005f };
			}
			GRAPHICS.RenderBox(position.x, position.y, position.z, 
				rotation.x, rotation.y, rotation.z,
				scaling.x, scaling.y, scaling.z );
		}
	}
}