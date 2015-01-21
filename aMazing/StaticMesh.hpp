#pragma once
#include"BasicObject.h"
#include"GPUVerticesBuffer.hpp"

template<typename T>
class StaticMesh : public BasicObject
{
public:
	StaticMesh(){}
	~StaticMesh(){}

	HRESULT Initialize(ID3D11Device* device, 
		ID3D11DeviceContext* context,
		T* verticesData,
		std::size_t verticesCount,
		WORD* indices = nullptr,
		std::size_t indicesCount = 0)
	{
		HRESULT hr;
		hr = BasicObject::Initialize(device, context);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = vertices.Initialize(device,context,verticesData,verticesCount,indices,indicesCount);
		if (FAILED(hr))
		{
			return hr;
		}
		return S_OK;
	}

	void Render(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		BasicObject::UpdatePRS(device, context);
		vertices.Render(device,context);
	}
private:
	GPUVerticesBuffer<T> vertices;
};