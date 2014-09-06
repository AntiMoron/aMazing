#pragma once

#include<D3D11.h>
#include<D3DX11.h>
#include<D3DX10.h>
#include<iostream>

template<typename type>
class GPUMutableVerticeBuffer
{
public:
	GPUMutableVerticeBuffer()
	{
		vertices = nullptr;
		indices = nullptr;
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
		if (vertices != nullptr)
		{
			vertices->Release();
			vertices = nullptr;
		}
		if (indices != nullptr)
		{
			indices->Release();
			indices = nullptr;
		}
		return S_OK;
	}

	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context,
		type* vexticesPtr,
		std::size_t vertexCount,
		WORD* indicesPtc = nullptr,
		std::size_t indexCount = 0)
	{
		HRESULT hr;
		D3D11_BUFFER_DESC vertDesc = {};
		D3D11_SUBRESOURCE_DATA verticeData = {};

		m_vertexCount = vertexCount;
		vertDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertDesc.ByteWidth = sizeof(type)*vertexCount;
		vertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertDesc.MiscFlags = 0;
		vertDesc.StructureByteStride = 0;
		vertexData.pSysMem = (void*)vexticesPtr;
		hr = device->CreateBuffer(&vertDesc, &vertexData, &m_vertices);
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
			D3D11_SUBRESOURCE_DATA indexData = {};

			m_indexCount = indexCount;
			indexDesc.Usage = D3D11_USAGE_DYNAMIC;
			indexDesc.ByteWidth = sizeof(WORD)* indexCount;
			indexDesc.CPUAccessFlags = 0;
			indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			indexData.pSysMem = indicesPtr;
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

			hr = device->CreateBuffer(&indexDesc, &indexData, &m_indices);
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
		type* vextices,
		std::size_t vertexCount)
	{
		HRESULT hr;
		D3D11_MAPPED_SUBRESOURCE mappedResource = {};

		m_vertexCount = vertexCount;
		hr = context->Map(m_vertices, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
			return hr;
		type* verticesPtr = (type*)mappedResource.pData;
		memcpy(verticesPtr, (void*)vertices, (sizeof(type)*m_vertexCount));
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
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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