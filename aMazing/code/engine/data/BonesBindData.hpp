#pragma once
#include<assimp/matrix4x4.h>

#define MAX_BONE 128
class BonesBindData
{
public:
	aiMatrix4x4 boneMatrices[MAX_BONE];
};
