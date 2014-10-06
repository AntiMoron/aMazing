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

bool& Maze::get(std::size_t x, std::size_t y)
{
	return m_map[x * width + y];
}

void Maze::Render(ID3D11Device* device,
	ID3D11DeviceContext* context,
	CameraClass* camera)
{
	int cnt = 0;
	float blockSize = 0.0005f;
	for (int p = 0; p < height; ++p)
	{
		for (int k = 0; k < width; ++k)
		{
			XMFLOAT3 position = { p * blockSize,
				blockSize,
				k * blockSize};
			XMFLOAT3 rotation = { .0f, .0f, .0f };
			XMFLOAT3 scaling = { blockSize, blockSize, blockSize };
			if (m_map[p * width + k] == false)
			{
				position.y = 0.0f;
			}
			//do frustum test on each block
			if (camera->getFrustum()->CheckCube(position.x, position.y, position.z,
				scaling.x / 2.0f, scaling.y / 2.0f, scaling.z / 2.0f))
			{
				GRAPHICS.RenderBox(position.x, position.y, position.z,
					rotation.x, rotation.y, rotation.z,
					scaling.x, scaling.y, scaling.z);
				++cnt;
			}
		}
	}
	printf("%d\n", cnt);
}