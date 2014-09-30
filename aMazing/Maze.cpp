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
	int chunkSize = 16;
	float blockSize = 0.005f;
	int halfx = width / 2;
	int halfy = height / 2;
	int chunkXCnt = width / chunkSize;
	int chunkYCnt = height / chunkSize;
	float xOffSet = halfx * blockSize;
	float yOffSet = halfy * blockSize;
	float halfChunkSize = float(chunkSize * blockSize) / 2.0f;
	auto frustum = camera->getFrustum();
	for (int i = 0 ; i <  chunkYCnt; ++i)
	{
		for (int j = 0; j < chunkXCnt; ++j)
		{
			//do frustum test on each chunk
			//chunk.x = maze.x/16 chunk.y = maze.y/16
			float chunk0_0x = float(j * chunkSize * blockSize);
			float chunk0_0y = float(i * chunkSize * blockSize);

			if (frustum->CheckCube(float(j * chunkSize * blockSize) + halfChunkSize - yOffSet,
				blockSize,
				float(i * chunkSize * blockSize) + halfChunkSize - xOffSet,
				halfChunkSize,
				2.0f * blockSize,
				halfChunkSize))
			{
				for (int p = 0; p < chunkSize; ++p)
				{
					for (int k = 0; k < chunkSize; ++k)
					{
						XMFLOAT3 position = { chunk0_0x + k * blockSize - xOffSet,
							blockSize,
							chunk0_0y + p * blockSize - yOffSet };
						XMFLOAT3 rotation = { .0f, .0f, .0f };
						XMFLOAT3 scaling = { blockSize, blockSize, blockSize };
						if (m_map[p * width + k] == false)
						{
							position.y = 0.0f;
						}
						//do frustum test on each block of this chunk
						if (frustum->CheckCube(position.x, position.y, position.z,
							scaling.x / 2.0f, scaling.y / 2.0f, scaling.z / 2.0f))
						{
							GRAPHICS.RenderBox(position.x, position.y, position.z,
								rotation.x, rotation.y, rotation.z,
								scaling.x, scaling.y, scaling.z);
							++cnt;
						}
					}
				}

			}
		}
	}
	printf("%d\n", cnt);
}