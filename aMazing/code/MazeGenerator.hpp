#pragma once
#include<vector>
#include<cstdio>
#include<ctime>
#include"Maze.hpp"
#include"collision/CollisionWorld.hpp"

using namespace aMazing;
class MazeGenerator
{
public:
	std::shared_ptr<Maze> genMaze(int dimension, const std::shared_ptr<CollisionWorld>& collision);
	static MazeGenerator& getInstance();
private:
	MazeGenerator();
	~MazeGenerator();

	void genMazeRecuresively(const std::shared_ptr<Maze>& result, int px, int py);
private:
	bool checkPoint(const std::shared_ptr<Maze>& ref, const std::pair<int, int>& pt);
	bool checkPoint(const std::shared_ptr<Maze>& ref, int a, int b);
};

#define MAZEFACTORY (MazeGenerator::getInstance())
