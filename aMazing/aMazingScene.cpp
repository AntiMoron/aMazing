#include "aMazingScene.h"


aMazingScene::aMazingScene()
{
}


aMazingScene::~aMazingScene()
{
}


HRESULT aMazingScene::Initialize(ID3D11Device* device,
	ID3D11DeviceContext* context)
{
	HRESULT hr;
	//Generate a maze
	maze.reset(MAZEFACTORY.genMaze(50));
	
	glow.reset(new GlowEffect);
	hr = glow->Initialize(device, context);
	if (FAILED(hr))
	{
		return hr;
	}

	depthField.reset(new DepthField);
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
void aMazingScene::Shutdown()
{
	glow->Shutdown();
	depthField->Shutdown();
}

void aMazingScene::Render(D3DClass* d3dkit, CameraClass* camera)
{
	ID3D11Device* device = d3dkit->getDevice();
	ID3D11DeviceContext* context = d3dkit->getContext();
	auto mazeRender = [&](ID3D11Device* device,ID3D11DeviceContext* context)
	{
		SHADERS.bindPair("Basic2D", device, context);
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