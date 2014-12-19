#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <memory>

class BoneClass
{
public:
	//the offset matrix of this bone.
	aiMatrix4x4 offsetMatrix;
	//the transformation of this bone.
	aiMatrix4x4 finalTransformation;
	//which vertex is influenced and how much is it.
	std::vector<std::unique_ptr<aiVertexWeight> > weights;
};