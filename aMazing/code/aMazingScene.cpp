#include "aMazingScene.hpp"


aMazingScene::aMazingScene()
{
}


HRESULT aMazingScene::initialize(HWND hwnd, ID3D11Device* device)
{
	HRESULT hr;
	//initialize collision world.
	//must be called before generating a maze
	collisionWorld = std::make_shared<CollisionWorld>();
	collisionWorld->initialize();

	//Generate a maze
	maze = MAZEFACTORY.genMaze(50, collisionWorld);

	camera = std::make_shared<WrappedCamera>();
	hr = camera->initialize(device);
	if (FAILED(hr))
	{
		return hr;
	}
	
	dayTime = std::make_shared<DayNightClass>();
	hr = dayTime->initialize(device);
	if (FAILED(hr))
	{
		return hr;
	}

	fb = std::make_shared<FrameBuffer>();
	hr = fb->initialize(device, 1024, 768);
	if (FAILED(hr))
	{
		return hr;
	}

	XMFLOAT2 twoDPosition = maze->getPositionByCoord(maze->getRandomEmptyCoord());
	XMFLOAT3 camerInitialPosition = { twoDPosition.x ,Maze::blockSize , twoDPosition.y};
	camera->setPosition(camerInitialPosition);
	camera->setFov(60.0f);

	model = std::make_shared<ModelObject>();
	hr = model->initialize(device, "3dModel/figure.fbx");
	if (FAILED(hr))
	{
		return hr;
	}

	classInstanceBuffer.reset(new GPUConstantBuffer<ClassInstance>);
	classInstanceBuffer->initialize(device, 11);
	return S_OK;
}

void aMazingScene::render(ID3D11DeviceContext* context)
{
	ClassInstance cbdata;
	cbdata.ambientLighting.vLightColor = { 1.0f, 1.0f, 1.0f };
	cbdata.directLighting.isEnable = true;
	cbdata.directLighting.vLightColor = { 1.0f, 1.0f, 1.0f };
	cbdata.directLighting.vLightDir = { 1.0f, 1.0f, 1.0f, 1.0f };
	cbdata.environmentLighting.isEnable = true;
	cbdata.environmentLighting.vLightColor = { 0.0f, 0.6353f, 0.910f };
	cbdata.material.iSpecFactor = 128;
	cbdata.material.vColor = { 0.5f, 0.5f, 0.5f };
	classInstanceBuffer->updateData(&cbdata);
	classInstanceBuffer->updateGpu(context);
	classInstanceBuffer->bindPixelShader(context);

	camera->render(context);
	dayTime->UpdateTime(context);

	XMFLOAT3 pos = camera->getPosition();
	//collisionWorld->updateCameraPos(pos.x, pos.z, 0.0f);
	//collisionWorld->getNewState();
	auto mazeRender = [&](ID3D11DeviceContext* context)
	{
		SHADERS.push("BasicSky", context);
		TEXTURE.getTexture(3)->bindPS(context, 0);
		GRAPHICS.renderRectangle(0, 0, WINWIDTH, WINHEIGHT);
		SHADERS.push("CameraDepth", context);
		maze->Render(context, camera->getCamera());
		//bind shader which skin animation need.
		if (model->isStatic())
		{
			SHADERS.push("Basic3D", context);
		}
		else
		{
			SHADERS.push("SkinAnim", context);
		}
		model->setRotation(XMFLOAT3(1.57, 0, 0));
		model->setScaling(XMFLOAT3(0.1f, 0.1f, 0.1f));
		model->render(context);
		GRAPHICS.renderLine(10.0f, 10.0f, 10.0f, 0.0f, 0.0f, 0.0f);
		SHADERS.pop(context);
		SHADERS.pop(context);
		SHADERS.pop(context);
	};
	fb->clearRenderTarget(context);
	fb->setRenderTarget(context);
	mazeRender(context);
	fb->bindPS(D3DManager::getContext(DEFAULT_CONTEXT),0);
	D3DManager::setMainRenderTarget();
	GRAPHICS.renderRectangle(0, 0, WINWIDTH, WINHEIGHT);

	GRAPHICS.renderText(0, 0, L"Ã´Ã´ßÕdadadada~", 72);
}

const std::shared_ptr<WrappedCamera>& aMazingScene::getWrappedCamera()
{
	return camera;
}
