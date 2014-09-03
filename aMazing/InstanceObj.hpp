#pragma once

//to make object instance.
//gives Position Rotation Scaling
#include<xnamath.h>

class PRS
{
public:
	PRS(){}
	PRS(const XMMATRIX&& pos,const XMMATRIX&& rot,const XMMATRIX&& sca)
		:position(pos),rotation(rot),scaling(sca){}
	XMMATRIX position;
	XMMATRIX rotation;
	XMMATRIX scaling;
};