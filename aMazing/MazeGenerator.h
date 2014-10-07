#pragma once
#include<set>
#include<vector>
#include<cstdio>
#include<ctime>
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

	void genMazeRecuresion(Maze* result,int px, int py);
	static MazeGenerator* instance;
private :

	bool checkPoint(Maze* ref, const std::pair<int, int>& pt);
	bool checkPoint(Maze* ref, int a, int b);
};

#define MAZEFACTORY (MazeGenerator::getInstance())