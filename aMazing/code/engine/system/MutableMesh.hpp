#pragma once

#include"BasicObject.h"
#include"GPUMutableVerticesBuffer.hpp"

template<typename T>
class MutableMesh
{
public:
	MutableMesh(){}
	~MutableMesh(){}

	HRESULT Initialize(ID3D11Device* device, 
		ID3D11DeviceContext* context,
		T* vertices,
		std::size_t vertexCount,
		WORD* indices = nullptr,
		std::size_t indexCount = 0)
	{
		HRESULT hr;
		hr = vertices.Initialize(device, context,vertices,vertexCount,indices,indexCount);
		if (FAILED(hr))
		{
			return hr;
		}
		return S_OK;
	}

	HRESULT updateVertices(ID3D11Device* device,
		ID3D11DeviceContext* context,
		T vertices,
		std::size_t vertexCount)
	{
		return vertices.UpdateVertices(device,context,vertices,vertexCount);
	}

	HRESULT updateIndices(ID3D11Device* device,ID3D11DeviceContext* context,WORD* indices,std::size_t indicesCount)
	{
		return vertices.UpdateIndices(device, context, indices, indicesCount);
	}

	void Render(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		vertices.Render(device, context);
	}
private:
	GPUMutableVerticeBuffer<T> vertices;
};