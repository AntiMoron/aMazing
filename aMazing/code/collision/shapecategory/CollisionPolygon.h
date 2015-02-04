#pragma once
#include<array>
#include<vector>
#include"CollisionShape.hpp"

//Unfinished
class CollisionPolygon : public CollisionShape
{
public:
	CollisionPolygon()
	{
		vertices = nullptr;
		vertexCount = 0;
		type = shape_polygon;
	}

	template<const int Count>
	void setVertices(std::array<XMFLOAT2, Count> vertices){
		if (vertices != nullptr)
		{
			delete [] vertices;
			vertices = nullptr;
		}
		vertices = new XMFLOAT2[Count];
		for (int i = 0; i < Count; i++)
		{
			this->vertices[i] = vertices[i];
		}
		vertexCount = Count;
	}
	
	std::size_t getVertexCount;
private:
	std::size_t vertexCount;
	XMFLOAT2* vertices;
};
