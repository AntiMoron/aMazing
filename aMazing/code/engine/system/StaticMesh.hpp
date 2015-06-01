#pragma once
#include"BasicObject.hpp"
#include"GPUVerticesBuffer.hpp"

namespace aMazing
{
	template<typename T>
	class StaticMesh
	{
	public:
		HRESULT initialize(ID3D11Device* device,
			ID3D11DeviceContext* context,
			T* verticesData,
			std::size_t verticesCount,
			WORD* indices = nullptr,
			std::size_t indicesCount = 0)
		{
			HRESULT hr;
			hr = vertices.initialize(device, verticesData, verticesCount, indices, indicesCount);
			if (FAILED(hr))
			{
				return hr;
			}
			return S_OK;
		}

		void render(ID3D11DeviceContext* context)
		{
			vertices.render(context);
		}
	private:
		GPUVerticesBuffer<T> vertices;
	};
}
