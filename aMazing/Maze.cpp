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
	if (x < 0 || x >= height)
	{
		throw std::out_of_range("Maze : X index out of range\r\n");
	}
	if (y < 0 || y >= width)
	{
		throw std::out_of_range("Maze : Y index out of range\r\n");
	}
	return m_map[x * width + y];
}

void Maze::Render(ID3D11Device* device,
	ID3D11DeviceContext* context,
	CameraClass* camera)
{
	float blockSize = 0.005f;
	float xOffSet = blockSize * width / 2.0f;
	float yOffSet = blockSize * height / 2.0f;
	for (int p = 0; p < height; ++p)
	{
		for (int k = 0; k < width; ++k)
		{
			XMFLOAT3 position = { p * blockSize - xOffSet,
				blockSize,
				k * blockSize - yOffSet };
			XMFLOAT3 rotation = { .0f, .0f, .0f };
			XMFLOAT3 scaling = { blockSize, blockSize, blockSize };
			if (m_map[p * width + k] == false)
			{
				position.y = 0.0f;
			}
			//do frustum test on each block
//			if (camera->getFrustum()->CheckCube(position.x, position.y, position.z,
//				scaling.x , scaling.y, scaling.z))
			{
				if (position.y > 0.0f)
					TEXTURE.getTexture(1)->bindPS(device, context, 0);
				else
					TEXTURE.getTexture(0)->bindPS(device, context, 0);
				GRAPHICS.RenderBox(position.x, position.y, position.z,
					rotation.x, rotation.y, rotation.z,
					scaling.x, scaling.y, scaling.z);
			}
		}
	}
}