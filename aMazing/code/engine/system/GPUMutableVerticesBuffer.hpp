#pragma once

#include"../../common/CommonDxSupport.hpp"
#include"../../common/CommonDef.hpp"
#include<iostream>

template<typename source_type>
class GPUMutableVerticeBuffer
{
public:
	GPUMutableVerticeBuffer()
	{
		vertices = nullptr;
		indices = nullptr;
		vertexCount = size_t(-1);
		indexCount = size_t(-1);
		is_inited = false;
	}
	~GPUMutableVerticeBuffer()
	{
		aSAFE_RELEASE(vertices);
		aSAFE_RELEASE(indices);
	}
	
	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context,
		source_type* vexticesPtr,
		std::size_t vertexCount,
		WORD* indicesPtr = nullptr,
		std::size_t indexCount = 0)
	{
		HRESULT hr;
		D3D11_BUFFER_DESC vertDesc = {};
		D3D11_SUBRESOURCE_DATA verticeData = {};

		this->vertexCount = vertexCount;
		vertDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertDesc.ByteWidth = sizeof(source_type)*vertexCount;
		vertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertDesc.MiscFlags = 0;
		vertDesc.StructureByteStride = 0;
		verticeData.pSysMem = (void*)vexticesPtr;
		hr = device->CreateBuffer(&vertDesc, &verticeData, &vertices);
		if (FAILED(hr))
		{
			return hr;
		}
		if (indicesPtr != nullptr)
		{
			if (indexCount == 0)
			{
				return E_FAIL;
			}
			D3D11_BUFFER_DESC indexDesc = {};
			D3D11_SUBRESOURCE_DATA indicesData = {};

			this->indexCount = indexCount;
			indexDesc.Usage = D3D11_USAGE_DYNAMIC;
			indexDesc.ByteWidth = sizeof(WORD)* indexCount;
			indexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			indicesData.pSysMem = indicesPtr;

			hr = device->CreateBuffer(&indexDesc, &indicesData, &indices);
			if (FAILED(hr))
			{
				return hr;
			}
		}
		is_inited = true;
		return S_OK;
	}

	HRESULT UpdateVertices(ID3D11Device* device,
		ID3D11DeviceContext* context,
		source_type* _vertices,
		std::size_t vertexCount)
	{
		HRESULT hr;
		D3D11_MAPPED_SUBRESOURCE mappedResource = {};

		this->vertexCount = vertexCount;
		hr = context->Map(vertices, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			return hr;
		}
		source_type* verticesPtr = reinterpret_cast<source_type*>(mappedResource.pData);
		memcpy(verticesPtr, reinterpret_cast<void*>(_vertices), sizeof(source_type)* vertexCount);
		context->Unmap(vertices, 0);
		return S_OK;
	}

	HRESULT UpdateIndices(ID3D11Device* device,
		ID3D11DeviceContext* context, 
		WORD* _indices,
		std::size_t indicesCount)
	{
		HRESULT hr;
		D3D11_MAPPED_SUBRESOURCE mappedResource = {};

		this->indexCount = indicesCount;
		hr = context->Map(indices, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
			return hr;
		WORD* indicesPtr = reinterpret_cast<VertexClass*>(mappedResource.pData);
		memcpy(indicesPtr, reinterpret_cast<void*>(_indices), sizeof(WORD)* indexCount);
		context->Unmap(indices, 0);
		return S_OK;
	}

	bool isInited()const
	{
		return is_inited;
	}

	void Bind(ID3D11Device* device,
		ID3D11DeviceContext* context)
	{
		UINT stride = sizeof(source_type);
		UINT offset = 0;
		if (indices != nullptr)
		{
			context->IASetIndexBuffer(indices, DXGI_FORMAT_R16_UINT, 0);
		}
		context->IASetVertexBuffers(0, 1, &vertices, &stride, &offset);
	}

	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context)
	{
		Bind(device, context);
		if (indices != nullptr)
		{
			context->DrawIndexed(indexCount,0,0);
		}
		else
		{
			context->Draw(vertexCount, 0);
		}
	}


private:
	volatile bool is_inited;
	std::size_t vertexCount;
	std::size_t indexCount;
	ID3D11Buffer* vertices;
	ID3D11Buffer* indices;
};
