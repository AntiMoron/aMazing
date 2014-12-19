#pragma once
#include<d3d11.h>
#include<xnamath.h>
#include<cmath>
#include<string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include"CommonTemplates.hpp"
#include"CommonDef.hpp"

//将值val按比例增长
float increaseByScale(float val,float scale,float upperBound,float bias);

//将值val按比例衰减
float fadeByScale(float val,float scale,float lowerBound,float bias);

//将值val按数值增长
float increaseByValue(float val, float increasement, float upperBound);

//将值val按照数值减少
float fadeByValue(float val, float decreasement, float lowerBound);

//将三维向量单位化
XMFLOAT3 normalize(XMFLOAT3 src);

class aiStringLess
{
public:
	bool operator () (const aiString& aiA,const aiString& aiB) const;
};