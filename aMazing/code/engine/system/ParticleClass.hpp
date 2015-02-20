#pragma once
#include<memory>
#include"CameraClass.hpp"
#include"GPUConstantBuffer.hpp"
#include<xnamath.h>
namespace aMazing
{
	class ParticleClass
	{
	public:
		ParticleClass();
		~ParticleClass();

		HRESULT Initialize(ID3D11Device* device,
			ID3D11DeviceContext* context);

		void Render(ID3D11Device* device,
			ID3D11DeviceContext* context,
			CameraClass* camera);
	private:
		struct ParticleMatrices
		{
			XMMATRIX inverseMatrix;
			XMFLOAT4 particleColor;
		};
		std::unique_ptr<GPUConstantBuffer<ParticleMatrices> > particleMatrices;
	};
}
