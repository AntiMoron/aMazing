#pragma once

#include<xnamath.h>


class vertex
{
public:
	vertex()
	{
		position = { .0f, .0f, .0f };
		texture = {.0f,.0f}; 
	}
	vertex(XMFLOAT3 pos,XMFLOAT2 tex):position(pos),texture(tex){}
	~vertex(){}
	vertex& operator = (const vertex& other)
	{
		position = other.position;
		texture = other.texture;
		return *this;
	}
	XMFLOAT3 position;
	XMFLOAT2 texture;
};