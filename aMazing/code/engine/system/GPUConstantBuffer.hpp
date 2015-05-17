#pragma once
#include"../../common/CommonDef.hpp"
#include"../../common/CommonDxSupport.hpp"

template<typename source_type>
class GPUConstantBuffer
{
public:
	GPUConstantBuffer()
	{
		buffer = nullptr;
	}
	~GPUConstantBuffer()
	{
		aSAFE_RELEASE(buffer);
	}
	HRESULT initialize(ID3D11Device* device, 
		int startSlot)
	{
		HRESULT hr;
		m_statSlotInGpu = startSlot;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(source_type);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = device->CreateBuffer(&bd, NULL, &buffer);
		if (FAILED(hr))
		{
			printf("CreateConstant Buffer Failed\r\n");
			return hr;
		}
		return S_OK;
	}

	HRESULT updateData(source_type* p)
	{
		memcpy(&data, p, sizeof(source_type));
		return S_OK;
	}

	HRESULT updateGpu(ID3D11DeviceContext* context)
	{
		context->UpdateSubresource(buffer, 0, NULL, &data, 0, 0);
		return S_OK;
	}
	HRESULT BindVertexShader(ID3D11DeviceContext* context)
	{
		context->VSSetConstantBuffers(m_statSlotInGpu, 1, &buffer);
		return S_OK;
	}

	HRESULT bindPixelShader(ID3D11DeviceContext* context)
	{
		context->PSSetConstantBuffers(m_statSlotInGpu, 1, &buffer);
		return S_OK;
	}
private:
	int m_statSlotInGpu;
	source_type data;			//要绑定的数据
	ID3D11Buffer* buffer;		//ConstantBuffer
};
