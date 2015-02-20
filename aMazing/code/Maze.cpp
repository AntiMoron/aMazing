#include "Maze.hpp"

const float Maze::blockSize = 0.5f;

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
	aMazing::CameraClass* camera)
{
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
					TEXTURE.getTexture(4)->bindPS(device, context, 0);
				GRAPHICS.RenderBox(position.x, position.y, position.z,
					rotation.x, rotation.y, rotation.z,
					scaling.x, scaling.y, scaling.z);
			}
		}
	}
}

std::pair<int,int> Maze::getRandomEmptyCoord()
{
	std::pair<int,int> result;
	int rx = rand() % height;
	int ry = rand() % width;
	while(get(rx , ry) == true)
	{
		rx = rand() % width;
		ry = rand() % height;
	}
	result.first = rx;
	result.second = ry;
	return result;
}

XMFLOAT2 Maze::getPositionByCoord(std::pair<int,int> coord)
{
	XMFLOAT2 result;
	float xOffSet = blockSize * width / 2.0f;
	float yOffSet = blockSize * height / 2.0f;
	result.x = coord.first * blockSize - xOffSet;
	result.y = coord.second * blockSize - yOffSet;
	return result;
}
