#pragma once
#include"PrimitivePipeline.h"
#include"TextureManager.h"
#include"CameraClass.h"

class Maze
{
public:
	~Maze();
	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		CameraClass* camera);

	bool& get(std::size_t x,std::size_t y);

	//get an empty place coord randomly
	std::pair<int,int> getRandomEmptyCoord();

	//get Position by coord
	XMFLOAT2 getPositionByCoord(std::pair<int, int> coord);
	const static float blockSize;
private:
	Maze();
	
	friend class MazeGenerator;
	bool* m_map;
	int width, height;
};