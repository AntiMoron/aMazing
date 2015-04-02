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
	/*
	get the wrapped camera
	*/
	const std::shared_ptr<aMazing::CameraClass>& getCamera();
	/*
	get the speed of camera moving.
	*/
	XMFLOAT3 getSpeed();
	/*
	get the position of camera.
	*/
	XMFLOAT3 getPosition();
	/*
	To initialize the camera.
	*/
	HRESULT Initialize(ID3D11Device* device,ID3D11DeviceContext* context);
	/*
	To set the camera's fov value.
	*/
	void setFov(float _fov);
	/*
	To set the camera's position value.
	*/
	void setPosition(XMFLOAT3 pos);
	/*
	To set the camera's step value.
	*/
	void setStep(float _step);
	/*
	To set the time per frame passes.
	*/
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
	std::shared_ptr<aMazing::CameraClass> camera;
	bool walking;
	float angle;
};
