#pragma once

//to make object instance.
//gives Position Rotation Scaling
#include<xnamath.h>
#include"../data/Vertex.hpp"
class PRS
{
public:
	PRS(){}
	PRS(XMFLOAT4X4&& pos, XMFLOAT4X4&& rot, XMFLOAT4X4&& sca)
		:position(pos),rotation(rot),scaling(sca){}
	~PRS(){}
	XMFLOAT4X4 position;
	XMFLOAT4X4 rotation;
	XMFLOAT4X4 scaling;
};

class PRSShaderData
{
public:
	XMMATRIX position;
	XMMATRIX rotation;
	XMMATRIX scaling;
};
