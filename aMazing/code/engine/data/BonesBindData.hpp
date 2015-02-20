#pragma once

#include<D3D11.h>
#include<D3DX11.h>
#include<xnamath.h>
#include<assimp/matrix4x4.h>

#define MAX_BONE 128
class BonesBindData
{
public:
	aiMatrix4x4 boneMatrices[MAX_BONE];
};
