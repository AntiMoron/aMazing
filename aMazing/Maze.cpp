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
	ID3D11DeviceContext* context,
	BlockClass* block)
{
	for (int i = 0; i<height ; i++)
	{
		for (int j = 0; j < width; j++)
		{
			XMFLOAT3 position = { i * 0.5f, 0.5f, j*0.5f };
			XMFLOAT3 rotation = { .0f, .0f, .0f };
			XMFLOAT3 scaling = { 0.5f, 0.5f, 0.5f };
			if (m_map[i * width + j] == true)
			{
				position = { i * 0.5f, 0.5f, j*0.5f };
			}
			else
			{
				position = { i * 0.5f, 0.0f, j*0.5f };
			}
			block->setRotation(rotation);
			block->setPosition(position);
			block->setScaling(scaling);
			block->Render(device, context);
		}
	}
}