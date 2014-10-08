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

	static MazeGenerator* instance;
	void genMazeRecuresion(Maze* result, int px, int py);
private :
	bool checkPoint(Maze* ref, const std::pair<int, int>& pt);
	bool checkPoint(Maze* ref, int a, int b);
};

#define MAZEFACTORY (MazeGenerator::getInstance())