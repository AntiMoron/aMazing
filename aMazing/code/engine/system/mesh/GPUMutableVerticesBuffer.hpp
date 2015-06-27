#pragma once
#include"../../render/RenderStateManager.hpp"
#include"BaseMesh.hpp"


namespace aMazing
{
	template<typename source_type>
	class GPUMutableVerticesBuffer : public BaseMesh<source_type>
	{
	public:
		HRESULT initialize(ID3D11Device* device,
			source_type* verticesPtr,
			size_t vertCount,
			WORD* indicesPtr = nullptr,
			size_t indCount = 0)
		{
			HRESULT hr;
			D3D11_BUFFER_DESC vertDesc = {};
			D3D11_SUBRESOURCE_DATA verticeData = {};

			verticesCount = vertCount;
			vertDesc.Usage = D3D11_USAGE_DYNAMIC;
			vertDesc.ByteWidth = sizeof(source_type) * verticesCount;
			vertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			vertDesc.MiscFlags = 0;
			vertDesc.StructureByteStride = 0;
			verticeData.pSysMem = (void*)verticesPtr;
			hr = device->CreateBuffer(&vertDesc, &verticeData, &verticesBuffer);
			if (FAILED(hr))
			{
				return hr;
			}
			if (indicesPtr != nullptr)
			{
				if (indCount == 0)
				{
					return E_FAIL;
				}
				D3D11_BUFFER_DESC indexDesc = {};
				D3D11_SUBRESOURCE_DATA indicesData = {};

				indicesCount = indCount;
				indexDesc.Usage = D3D11_USAGE_DYNAMIC;
				indexDesc.ByteWidth = sizeof(WORD) * indicesCount;
				indexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

				indicesData.pSysMem = indicesPtr;

				hr = device->CreateBuffer(&indexDesc, &indicesData, &indicesBuffer);
				if (FAILED(hr))
				{
					return hr;
				}
			}
			bIsInited = true;
			return S_OK;
		}

		HRESULT updateVertices(ID3D11DeviceContext* context,
			source_type* _vertices,
			std::size_t vertCount)
		{
			HRESULT hr;
			D3D11_MAPPED_SUBRESOURCE mappedResource = {};

			verticesCount = vertCount;
			hr = context->Map(verticesBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(hr))
			{
				return hr;
			}
			source_type* verticesPtr = reinterpret_cast<source_type*>(mappedResource.pData);
			memcpy(verticesPtr, reinterpret_cast<void*>(_vertices), sizeof(source_type)* vertCount);
			context->Unmap(verticesBuffer, 0);
			return S_OK;
		}

		HRESULT updateIndices(ID3D11DeviceContext* context, 
			WORD* _indices,
			std::size_t indCount)
		{
			HRESULT hr;
			D3D11_MAPPED_SUBRESOURCE mappedResource = {};

			indicesBuffer = indCount;
			hr = context->Map(indicesBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(hr))
				return hr;
			WORD* indicesPtr = reinterpret_cast<VertexClass*>(mappedResource.pData);
			memcpy(indicesPtr, reinterpret_cast<void*>(_indices), sizeof(WORD)* indexCount);
			context->Unmap(indicesBuffer, 0);
			return S_OK;
		}

		void bind(ID3D11DeviceContext* context)
		{
			UINT stride = sizeof(source_type);
			UINT offset = 0;
			if (indicesBuffer != nullptr)
			{
				context->IASetIndexBuffer(indicesBuffer, DXGI_FORMAT_R16_UINT, 0);
			}
			context->IASetVertexBuffers(0, 1, &verticesBuffer, &stride, &offset);
		}

		void render(ID3D11DeviceContext* context)
		{
			if (RENDER_STATE_MANAGER.isWireframe())
			{
				context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
			}
			else
			{
				context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			}
			bind(context);
			if (indicesBuffer != nullptr)
			{
				context->DrawIndexed(indicesCount, 0, 0);
			}
			else
			{
				context->Draw(verticesCount, 0);
			}
		}
	};
}
