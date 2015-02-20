#pragma once
#include<memory>
#include"common/CommonUtil.hpp"
#include"engine/system/CameraClass.hpp"
#include"Maze.hpp"

class WrappedCamera
{
public:
	WrappedCamera();
	~WrappedCamera();
	aMazing::CameraClass* getCamera();
	XMFLOAT3 getSpeed();
	XMFLOAT3 getPosition();
	HRESULT Initialize(ID3D11Device* device,ID3D11DeviceContext* context);

	void setFov(float _fov);
	void setPosition(XMFLOAT3 pos);
	void setStep(float _step);
	void SetFrameTime(std::size_t _frameTime);
	void moveForward(bool keyDown);
	void moveBackward(bool keyDown);
	void moveLeftward(bool keyDown);
	void moveRightward(bool keyDown);
	void Render(ID3D11Device* device, ID3D11DeviceContext* context);
private:
	float step;
	XMFLOAT3 speed;
	XMFLOAT3 negativeSpeed;
	XMFLOAT3 walkBias;	//the bias rotation made while walking around
	std::size_t frameTime;
	std::unique_ptr<aMazing::CameraClass> camera;
	bool walking;
	float angle;
};
