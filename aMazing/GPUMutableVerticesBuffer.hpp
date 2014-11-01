#pragma once

#include<D3D11.h>
#include<D3DX11.h>
#include<D3DX10.h>
#include<iostream>

template<typename source_type>
class GPUMutableVerticeBuffer
{
public:
	GPUMutableVerticeBuffer()
	{
		m_vertices = nullptr;
		m_indices = nullptr;
		m_vertexCount = -1;
		m_indexCount = -1;
		is_inited = false;
	}
	~GPUMutableVerticeBuffer()
	{
		Shutdown();
	}
	
	HRESULT Shutdown()
	{
		if (m_vertices != nullptr)
		{
			m_vertices->Release();
			m_vertices = nullptr;
		}
		if (m_indices != nullptr)
		{
			m_indices->Release();
			m_indices = nullptr;
		}
		return S_OK;
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

		m_vertexCount = vertexCount;
		vertDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertDesc.ByteWidth = sizeof(source_type)*vertexCount;
		vertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertDesc.MiscFlags = 0;
		vertDesc.StructureByteStride = 0;
		verticeData.pSysMem = (void*)vexticesPtr;
		hr = device->CreateBuffer(&vertDesc, &verticeData, &m_vertices);
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

			m_indexCount = indexCount;
			indexDesc.Usage = D3D11_USAGE_DYNAMIC;
			indexDesc.ByteWidth = sizeof(WORD)* indexCount;
			indexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			indicesData.pSysMem = indicesPtr;

			hr = device->CreateBuffer(&indexDesc, &indicesData, &m_indices);
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
		source_type* vertices,
		std::size_t vertexCount)
	{
		HRESULT hr;
		D3D11_MAPPED_SUBRESOURCE mappedResource = {};

		m_vertexCount = vertexCount;
		hr = context->Map(m_vertices, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			return hr;
		}
		source_type* verticesPtr = (source_type*)mappedResource.pData;
		memcpy(verticesPtr, (void*)vertices, (sizeof(source_type)* m_vertexCount));
		context->Unmap(m_vertices, 0);
		return S_OK;
	}

	HRESULT UpdateIndices(ID3D11Device* device,
		ID3D11DeviceContext* context, 
		WORD* indices,
		std::size_t indicesCount)
	{
		HRESULT hr;
		D3D11_MAPPED_SUBRESOURCE mappedResource = {};

		m_indexCount = indicesCount;
		hr = context->Map(m_indices, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
			return hr;
		WORD* indicesPtr = (VertexClass*)mappedResource.pData;
		memcpy(indicesPtr, (void*)indices, (sizeof(WORD)*m_indexCount));
		context->Unmap(m_indices, 0);
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
		if (m_indices != nullptr)
		{
			context->IASetIndexBuffer(m_indices, DXGI_FORMAT_R16_UINT, 0);
		}
		context->IASetVertexBuffers(0, 1, &m_vertices, &stride, &offset);
	}

	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context)
	{
		Bind(device, context);
		if (m_indices != nullptr)
		{
			context->DrawIndexed(m_indexCount,0,0);
		}
		else
		{
			context->Draw(m_vertexCount, 0);
		}
	}


private:
	bool is_inited;
	std::size_t m_vertexCount;
	std::size_t m_indexCount;
	ID3D11Buffer* m_vertices;
	ID3D11Buffer* m_indices;
};