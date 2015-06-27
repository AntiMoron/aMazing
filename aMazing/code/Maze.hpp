#pragma once
#include"engine/system/primitive/PrimitivePipeline.hpp"
#include"engine/system/TextureManager.hpp"
#include"engine/system/CameraClass.hpp"

class Maze
{
public:
	void Render(ID3D11DeviceContext* context,
		const std::shared_ptr<aMazing::CameraClass>& camera);

	bool& get(std::size_t x,std::size_t y);

	//get an empty place coord randomly
	std::pair<int,int> getRandomEmptyCoord();

	//get Position by coord
	XMFLOAT2 getPositionByCoord(std::pair<int, int> coord);
	const static float blockSize;
private:
	Maze();
	
	friend class MazeGenerator;
	std::unique_ptr<bool[]> m_map;
	int width, height;
};
