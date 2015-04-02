#include "aMazingScene.hpp"


aMazingScene::aMazingScene()
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
	maze.reset(MAZEFACTORY.genMaze(50, collisionWorld));

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

	dayTime.reset(new DayNightClass);
	hr = dayTime->Initialize(device, context);
	if (FAILED(hr))
	{
		return hr;
	}

	shadow.reset(new ShadowMap);
	hr = shadow->Initialize(device, context);
	if (FAILED(hr))
	{
		return hr;
	}

	XMFLOAT2 twoDPosition = maze->getPositionByCoord(maze->getRandomEmptyCoord());
	XMFLOAT3 camerInitialPosition = { twoDPosition.x ,Maze::blockSize , twoDPosition.y};
	camera->setPosition(camerInitialPosition);
	camera->setFov(60.0f);


	model.reset(new ModelObject);
	/*hr = model->Initialize(device, context, "C:/Users/Think/Desktop/DEMO/scene/kyjn/kyjn01/1-2/3_2.obj");*/
	hr = model->Initialize(device, context, "3dModel/figure.fbx");
	if (FAILED(hr))
	{
		return hr;
	}

	classInstanceBuffer.reset(new GPUConstantBuffer<ClassInstance>);
	classInstanceBuffer->Initialize(device, context, 11);
	return S_OK;
}

void aMazingScene::Render(ID3D11Device* device, ID3D11DeviceContext* context)
{

	ClassInstance cbdata;
	cbdata.ambientLighting.vLightColor = { 1.0f, 1.0f, 1.0f };
	cbdata.directLighting.isEnable = true;
	cbdata.directLighting.vLightColor = { 1.0f, 1.0f, 1.0f };
	cbdata.directLighting.vLightDir = { 1.0f, 1.0f, 1.0f, 1.0f };
	cbdata.environmentLighting.isEnable = true;
	cbdata.environmentLighting.vLightColor = { 1.0f, 1.0f, 1.0f };
	cbdata.material.iSpecFactor = 128;
	cbdata.material.vColor = { 0.5f, 0.5f, 0.5f };
	classInstanceBuffer->UpdateData(&cbdata);
	classInstanceBuffer->UpdateGpu(device, context);
	classInstanceBuffer->BindPixelShader(device, context);


	camera->Render(device, context);
	dayTime->UpdateTime(device, context);

	XMFLOAT3 pos = camera->getPosition();
	//collisionWorld->updateCameraPos(pos.x, pos.z, 0.0f);
	//collisionWorld->getNewState();
	
	auto mazeRender = [&](ID3D11Device* device, ID3D11DeviceContext* context)
	{
		SHADERS.bindPair("BasicSky", device, context);
		TEXTURE.getTexture(3)->bindPS(device, context, 0);
		GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
		SHADERS.bindPair("Basic3D", device, context);
		maze->Render(device, context, camera->getCamera());
		//bind shader which skin animation need.

		if (model->isStatic())
		{
			SHADERS.bindPair("Basic3D", device, context);
		}
		else
		{
			SHADERS.bindPair("SkinAnim", device, context);
		}
		model->setRotation(XMFLOAT3(1.57, 0, 0));
		model->setScaling(XMFLOAT3(0.1f, 0.1f, 0.1f));
		model->Render(device, context);
	};
	mazeRender(device,context);
	//glow->Render(device, context, mazeRender);
	////glow->Render(device, context, shadowRender);
	//d3dkit->setRenderTarget();

	//SHADERS.bindPair("Basic2D", device, context);
	//shadow->bindPS(device, context, 0);
	//GRAPHICS.RenderRectangle(0, 0, WINWIDTH, WINHEIGHT);
}

const std::shared_ptr<WrappedCamera>& aMazingScene::getWrappedCamera()
{
	return camera;
}
