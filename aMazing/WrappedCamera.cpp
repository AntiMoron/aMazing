#include "WrappedCamera.h"

using namespace CommonFunction;
WrappedCamera::WrappedCamera()
{
	speed.x = 0.0f;
	speed.y = 0.0f;
	speed.z = 0.0f;
	walkBias.x = 0.0f;
	walkBias.y = 0.0f;
	walkBias.z = 0.0f;
	frameTime = 0.0f;
	negativeSpeed.x = 0.0f;
	negativeSpeed.y = 0.0f;
	negativeSpeed.z = 0.0f;
	angle = 0.0f;
	step = 1.0f;
	walking = false;
}


WrappedCamera::~WrappedCamera()
{
}



CameraClass* WrappedCamera::getCamera()
{
	return camera.get();
}

XMFLOAT3 WrappedCamera::getSpeed()
{
	return speed;
}

void WrappedCamera::setFov(float _fov)
{
	camera->setFov(_fov);
}

void WrappedCamera::setPosition(XMFLOAT3 pos)
{
	camera->setPosition(pos);
}


XMFLOAT3 WrappedCamera::getPosition()
{
	return camera->getPosition();
}

HRESULT WrappedCamera::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT hr;
	camera.reset(new CameraClass);
	hr = camera->Initialize(device, context);
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

void WrappedCamera::setStep(float _step)
{
	step = _step;
}
void WrappedCamera::SetFrameTime(std::size_t _frameTime)
{
	frameTime = _frameTime;
}
void WrappedCamera::moveForward(bool keyDown)
{
	if (keyDown)
	{
		speed.x = increaseByValue(speed.x, step * 0.05f, step);
		walking = true;
	}
	else
	{
		speed.x = fadeByScale(speed.x, 0.6, 0.0f, step * 0.01f);
	}
}

void WrappedCamera::moveBackward(bool keyDown)
{
	if (keyDown)
	{
		negativeSpeed.x = fadeByValue(negativeSpeed.x, step * 0.05f, -step);
		walking = true;
	}
	else
	{
		negativeSpeed.x = fadeByScale(negativeSpeed.x, 0.6, 0.0f, step * 0.01f);
	}
}
void WrappedCamera::moveLeftward(bool keyDown)
{
	if (keyDown)
	{
		speed.z = increaseByValue(speed.z, step * 0.05f, step);
		walking = true;
	}
	else
	{
		speed.z = fadeByScale(speed.z, 0.6, 0.0f, step * 0.01f);
	}
}

void WrappedCamera::moveRightward(bool keyDown)
{
	if (keyDown)
	{
		negativeSpeed.z = fadeByValue(negativeSpeed.z, step * 0.05f, -step);
		walking = true;
	}
	else
	{
		negativeSpeed.z = fadeByScale(negativeSpeed.z, 0.6, 0.0f, step * 0.01f);
	}
}

void WrappedCamera::Render(ID3D11Device* device, ID3D11DeviceContext* context)
{
	XMFLOAT3 _speed = speed;
	_speed.x += negativeSpeed.x;
	_speed.y += negativeSpeed.y;
	_speed.z += negativeSpeed.z;
	_speed = normalize(_speed);
	_speed.x *= step;
	_speed.y *= step;
	_speed.z *= step;
	if (speed.x + negativeSpeed.x == 0.0f &&
		speed.y + negativeSpeed.y == 0.0f &&
		speed.z + negativeSpeed.z == 0.0f)
	{
		walking = false;
	}

	if (walking)
	{
		angle += 6.0f;
		if (angle > 360.0f)
			angle = 0.0f;
	}
	else
	{
		angle = fadeByScale(angle, 0.6, 0.0f, 0.01f);
	}
	walkBias.y = 0.03f * sin(0.017453292f * angle * 2.0f) *  sin(0.017453292f * angle* 2.0f);
	walkBias.z = 0.01f * sin(0.017453292f * angle) * sin(0.017453292f * angle);
	
	camera->moveForward(_speed.x);
	camera->moveLeft(_speed.z);
	XMFLOAT3 rotation = camera->getRotation();
	walkBias.y += rotation.y;
	walkBias.z += rotation.z;
	//set camera rotation
	camera->setRotation(walkBias);
	//force camera position on the ground level
	XMFLOAT3 pos = camera->getPosition();
//	pos.y = Maze::blockSize * 1.05f;
//	camera->setPosition(pos);
	camera->Render(device, context);
	//reset rotation to origin for next calculation
	camera->setRotation(rotation);
}