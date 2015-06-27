#pragma once
#include"../../render/RenderStateManager.hpp"
#include"BaseMesh.hpp"

namespace aMazing
{
	template<typename source_type>
	class GPUVerticesBuffer : public BaseMesh<source_type>
	{
	public:
		//If the indices pointer is nullptr,then no index buffer would be initialized
		HRESULT initialize(ID3D11Device* device,
			source_type* verticesData, 
			size_t vertCount,
			WORD* indices = nullptr, 
			size_t indCount = 0)
		{
			HRESULT hr = S_OK;
			bIsInited = false;

			verticesCount = vertCount;
			indicesCount = indCount;

			indicesBuffer = nullptr;
			verticesBuffer = nullptr;
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(source_type)* verticesCount;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory(&InitData, sizeof(InitData));
			InitData.pSysMem = verticesData;
			hr = device->CreateBuffer(&bd, &InitData, &verticesBuffer);
			if (FAILED(hr))
			{
				aDBG("Vertex Buffer Create Failed\r\n");
				return hr;
			}
			// Set Vertex buffer

			if (nullptr != indices)
			{
				ZeroMemory(&bd, sizeof(bd));
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(WORD)* indicesCount;
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;

				ZeroMemory(&InitData, sizeof(InitData));
				InitData.pSysMem = indices;
				hr = device->CreateBuffer(&bd, &InitData, &indicesBuffer);
				if (FAILED(hr))
				{
					aDBG("Index buffer Create Failed.");
					return hr;
				}
			}

			bIsInited = true;
			return S_OK;
		}

		HRESULT bind(ID3D11DeviceContext* context)
			//Bind the vertice buffer and the index buffer(if it has one)
		{
			UINT stride = sizeof(source_type);
			UINT offset = 0;
			if (indicesBuffer != nullptr)
			{
				context->IASetIndexBuffer(indicesBuffer, 
					DXGI_FORMAT_R16_UINT, 0);
			}
			context->IASetVertexBuffers(0, 1, &verticesBuffer, &stride, &offset);
			return S_OK;
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
