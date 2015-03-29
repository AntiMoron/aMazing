#pragma once

#include<D3D11.h>
#include<D3DX11.h>
#include"../../common/CommonDef.hpp"

template<typename source_type>
class GPUVerticesBuffer
{
public:
	GPUVerticesBuffer()
	{
		m_index = nullptr;
		m_vertices = nullptr;
		m_inited = false;
		m_hasIndex = false;
	}
	~GPUVerticesBuffer()
	{
		aSAFE_RELEASE(m_vertices);
		aSAFE_RELEASE(m_index);
	}

	//If the indices pointer is nullptr,then no index buffer would be initialized
	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context, source_type* verticesData, int verticesCount, WORD* indices = nullptr, std::size_t indicesCount = 0)
	{
		HRESULT hr = S_OK;
		m_inited = false;
		m_hasIndex = false;

		m_verticesCount = verticesCount;
		m_indicesCount = indicesCount;

		m_index = nullptr;
		m_vertices = nullptr;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(source_type)* verticesCount;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = verticesData;
		hr = device->CreateBuffer(&bd, &InitData, &m_vertices);
		if (FAILED(hr))
		{
			printf("Vertex Buffer Create Failed\r\n");
			return hr;
		}
		// Set Vertex buffer
		UINT stride = sizeof(source_type);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &m_vertices, &stride, &offset);

		if (nullptr != indices)
		{
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(WORD)* indicesCount;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			ZeroMemory(&InitData, sizeof(InitData));
			InitData.pSysMem = indices;
			hr = device->CreateBuffer(&bd, &InitData, &m_index);
			if (FAILED(hr))
			{
				printf("Index buffer Create Failed\r\n");
				return hr;
			}
			context->IASetIndexBuffer(m_index, DXGI_FORMAT_R16_UINT, 0);
			m_hasIndex = true;
		}

		m_inited = true;
		return S_OK;
	}

	HRESULT Bind(ID3D11Device* device, ID3D11DeviceContext* context)
		//Bind the vertice buffer and the index buffer(if it has one)
	{
		UINT stride = sizeof(source_type);
		UINT offset = 0;
		if (m_hasIndex == true)
		{
			context->IASetIndexBuffer(m_index, DXGI_FORMAT_R16_UINT, 0);
		}
		context->IASetVertexBuffers(0, 1, &m_vertices, &stride, &offset);
		return S_OK;
	}

	void Render(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Bind(device, context);
		if (m_hasIndex == true)
		{
			context->DrawIndexed(m_indicesCount, 0, 0);
		}
		else
		{
			context->Draw(m_verticesCount, 0);
		}
	}

	bool isInited()const
	{
		return m_inited;
	}
private:

	bool m_inited;
	bool m_hasIndex;
	std::size_t m_verticesCount;
	std::size_t m_indicesCount;
	ID3D11Buffer* m_index;
	ID3D11Buffer* m_vertices;
};
