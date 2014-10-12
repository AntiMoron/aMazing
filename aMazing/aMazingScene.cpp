#include "aMazingScene.h"


aMazingScene::aMazingScene()
{
}


aMazingScene::~aMazingScene()
{
}


HRESULT aMazingScene::Initialize(HWND hwnd, ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	HRESULT hr;
	//Initialize collision world.
	//must be called before generating a maze
	collisionWorld.reset(new CollisionWorld);
	collisionWorld->Initialize();

	//Generate a maze
	maze.reset(MAZEFACTORY.genMaze(50, collisionWorld.get()));

	camera.reset(new WrappedCamera);
	hr = camera->Initialize(device, context);
	if (FAILED(hr))
	{
		return hr;
	}
	
	glow.reset(new GlowEffect);
	hr = glow->Initialize(device, context);
	if (FAILED(hr))
	{
		return hr;
	}

	sound.reset(new SoundClass);
	hr = sound->Initialize(hwnd, "bgm.ogg");
	if (FAILED(hr))
	{
		E_FAIL;
	}

	dayTime.reset(new DayNightClass);
	hr = dayTime->Initialize(device, context);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	XMFLOAT2 twoDPosition = maze->getPositionByCoord(maze->getRandomEmptyCoord());
	XMFLOAT3 camerInitialPosition = { twoDPosition.x ,Maze::blockSize , twoDPosition.y};
	camera->setPosition(camerInitialPosition);
	camera->setFov(60.0f);
	return S_OK;
}

void aMazingScene::Shutdown()
{
	if (glow.get() != nullptr)
	{
		glow->Shutdown();
	}
	if (sound.get() != nullptr)
	{
		sound->Shutdown();
	}
	if (collisionWorld.get() != nullptr)
	{
		collisionWorld->Shutdown();
	}
	if (dayTime.get() != nullptr)
	{
		dayTime->Shutdown();
	}
	if (camera.get() != nullptr)
	{
		camera->Shutdown();
	}
}

void aMazingScene::Render(D3DClass* d3dkit)
{
	sound->Play();
	ID3D11Device* device = d3dkit->getDevice();
	ID3D11DeviceContext* context = d3dkit->getContext();
	camera->Render(device, context);
	dayTime->UpdateTime(device, context);

	XMFLOAT3 pos = camera->getPosition();
	
	collisionWorld->updateCameraPos(pos.x, pos.z, 0.0f);

	collisionWorld->getNewState();
	
	auto mazeRender = [&](ID3D11Device* device, ID3D11DeviceContext* context)
	{
		SHADERS.bindPair("BasicSky", device, context);
		TEXTURE.getTexture(3)->bindPS(device, context, 0);
		GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
		SHADERS.bindPair("Basic3D", device, context);
		maze->Render(device, context, camera->getCamera());
	};

	glow->Render(device, context, mazeRender);
	d3dkit->setRenderTarget();

	SHADERS.bindPair("Basic2D", device, context);
	glow->bindPS(device, context, 0);
	GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
}

WrappedCamera* aMazingScene::getCamera()
{
	return camera.get();
}