#pragma once
#include<set>
#include<vector>
#include<cstdio>
#include<ctime>
#include"Maze.hpp"
#include"collision/CollisionWorld.hpp"

using namespace aMazing;
class MazeGenerator
{
public:
	Maze* genMaze(int dimension, const std::shared_ptr<CollisionWorld>& collision);
	static MazeGenerator& getInstance();
private:
	MazeGenerator();
	~MazeGenerator();

	void genMazeRecuresion(Maze* result, int px, int py);
private :
	bool checkPoint(Maze* ref, const std::pair<int, int>& pt);
	bool checkPoint(Maze* ref, int a, int b);
};

#define MAZEFACTORY (MazeGenerator::getInstance())
