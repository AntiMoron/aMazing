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
	if (dayTime.get() == nullptr)
	{
		dayTime->Shutdown();
	}
}

void aMazingScene::Render(D3DClass* d3dkit, CameraClass* camera)
{
	sound->Play();
	ID3D11Device* device = d3dkit->getDevice();
	ID3D11DeviceContext* context = d3dkit->getContext();
	dayTime->UpdateTime(device, context);

	XMFLOAT3 pos = camera->getPosition();
	
	collisionWorld->updateCameraPos(pos.x, pos.z, camera->getRotation().y);

	collisionWorld->getNewState();
	
	auto mazeRender = [&](ID3D11Device* device, ID3D11DeviceContext* context)
	{
		SHADERS.bindPair("BasicSky", device, context);
		TEXTURE.getTexture(3)->bindPS(device, context, 0);
		GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
		SHADERS.bindPair("Basic3D", device, context);
		maze->Render(device, context, camera);
	};

	glow->Render(device, context, mazeRender);
	
	d3dkit->setRenderTarget();

	SHADERS.bindPair("Basic2D", device, context);
	glow->bindPS(device, context, 0);
	GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
}