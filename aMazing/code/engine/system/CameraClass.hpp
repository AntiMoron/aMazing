#pragma once

#include<memory>
#include"GPUConstantBuffer.hpp"
#include<xnamath.h>
#include"WindowClass.hpp"
#include"Defines.hpp"
#include"FrustumClass.hpp"

namespace aMazing
{
	class CameraClass
	{
	public:
		CameraClass();
		~CameraClass();

		HRESULT Initialize(ID3D11Device* device,
			ID3D11DeviceContext* context);

		void setAspectRatio(float ratio);
		void setPosition(const XMFLOAT3& pos);
		void setPosition(const XMFLOAT3&& pos);
		XMFLOAT3 getPosition() const;
		void setRotation(const XMFLOAT3& rot);
		void setRotation(const XMFLOAT3&& rot);
		XMFLOAT3 getRotation() const;
		XMFLOAT3 getUpVector() const;
		void setNearDist(const float val);
		void setFarDist(const float val);
		float getNearDist() const;
		float getFarDist() const;
		void setFov(const float ang);
		float getFov() const;

		void Render(ID3D11Device* device,
			ID3D11DeviceContext* context);


		void moveLeft(float step);
		void moveForward(float step);
		void moveRight(float step);
		void moveBackward(float step);

		void turnLeft(float step);
		void turnRight(float step);
		void lookUp(float step);
		void lookDown(float step);
		void twistLeft(float );
		void twistRight(float);

		FrustumClass* getFrustum();
	private:
		float fov;
		XMFLOAT2 near_far;
		//near Dist and far Dist
		XMFLOAT3 position; 
		//camera Position
		XMFLOAT3 rotation; 
		// - x conterclockwise,vice versa.
		// - y down,vice versa.
		// - z left,vice versa.
		XMFLOAT3 upVector;

		XMFLOAT3 speed;
		XMFLOAT3 forwardDirection;
		XMFLOAT3 lefthandDirection;
		struct cameraMatrices
		{
			XMMATRIX world;
			XMMATRIX view;
			XMMATRIX projection;
			XMFLOAT3 eyeDirection;
		};
		struct cameraMatrices m_matriceData;
		GPUConstantBuffer<cameraMatrices> m_matrices;
		std::unique_ptr<FrustumClass> m_frustum;
	};
}
