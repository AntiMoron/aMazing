#pragma once
#include<xnamath.h>


class vertex
{
public:
	vertex()
	{
		position = { .0f, .0f, .0f };
		normal = { .0f, .0f, .0f }; 
		texture = {.0f,.0f}; 
	}
	vertex(XMFLOAT3 pos,XMFLOAT3 nor,XMFLOAT2 tex):position(pos),
		normal(nor),
		texture(tex){}
	~vertex(){}
	vertex& operator = (const vertex& other)
	{
		position = other.position;
		normal = other.normal;
		texture = other.texture;
		return *this;
	}
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texture;
};