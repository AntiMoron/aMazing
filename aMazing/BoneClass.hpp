#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <memory>

class BoneClass
{
public:
	aiMatrix4x4 boneOffSet;
	aiMatrix4x4 finalTransformation;
	std::vector<std::unique_ptr<aiVertexWeight> > weights;
};