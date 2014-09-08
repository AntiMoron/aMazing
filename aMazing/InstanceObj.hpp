#pragma once

//to make object instance.
//gives Position Rotation Scaling
#include<xnamath.h>
#include"vertex.hpp"
class PRS
{
public:
	PRS(){}
	PRS(const XMMATRIX&& pos,const XMMATRIX&& rot,const XMMATRIX&& sca)
		:position(pos),rotation(rot),scaling(sca){}
	~PRS(){}
	XMMATRIX position;
	XMMATRIX rotation;
	XMMATRIX scaling;
};