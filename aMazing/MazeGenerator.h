#pragma once
#include<vector>
#include"Maze.h"

class MazeGenerator
{
public:
	Maze* genMaze(int dimension);
	static MazeGenerator& getInstance();
private:
	MazeGenerator();
	~MazeGenerator();

	std::vector<std::vector<int> > MazeGenerator::GenerateWalls(int width, int height);

	static MazeGenerator* instance;
};

#define MAZEFACTORY (MazeGenerator::getInstance())