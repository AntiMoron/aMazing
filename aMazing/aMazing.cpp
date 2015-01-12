#include <windows.h>
#include "D3DClass.h"
#include "CameraClass.h"
#include "InputClass.h"
#include "resource.h"
#include "WindowClass.h"
#include "TextureManager.h"
#include "MazeGenerator.h"
#include "PrimitivePipeline.h"
#include "AmbientLight.h"
#include "aMazingScene.h"

HINSTANCE g_hInst = nullptr;
HWND g_hWnd = nullptr;

D3DClass d3d;
aMazingScene scene;

#define DEVICE (d3d.getDevice())
#define CONTEXT (d3d.getContext())
#define DEPTH (d3d.getDepthStencilView())

HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
void Render();

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	AllocConsole();
	FILE * m_new_stdout_file;
	freopen_s(&m_new_stdout_file, "CONOUT$", "w+t", stdout);
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            Render();
        }
    }

    CleanupDevice();

    return ( int )msg.wParam;
}

HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"aMazingWndClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"aMazingWndClass", L"aMazing by AntiMoron anti2moron@gmail.com", WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}

HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

	WindowClass::getInstance().setWidth(width);
	WindowClass::getInstance().setHeight(height);
	//Initialize all the thing we need to prepare for rendering work.
	d3d.Initialize(g_hWnd);
	
	// Load the Texture
	hr = TEXTURE.addTexture(DEVICE, CONTEXT, "seafloor.dds");
	if (FAILED(hr))
		return hr;

	hr = TEXTURE.addTexture(DEVICE, CONTEXT, "glowstone.png");
	if (FAILED(hr))
		return hr;

	hr = TEXTURE.addTexture(DEVICE, CONTEXT, "leaves.png");
	if (FAILED(hr))
		return hr;

	hr = TEXTURE.addTexture(DEVICE, CONTEXT, "sky.png");
	if (FAILED(hr))
		return hr;

	hr = TEXTURE.addChessBoardTexture(DEVICE, CONTEXT);
	if (FAILED(hr))
		return hr;

	//aditional operations.
	GRAPHICS.Initialize(&d3d);
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, aOffsetof(Vertex,position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, aOffsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, aOffsetof(Vertex, texture), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D11_INPUT_ELEMENT_DESC animLayout[] =
	{
		{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, aOffsetof(SkinVertex, boneIndices), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, aOffsetof(SkinVertex, weights), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, aOffsetof(SkinVertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, aOffsetof(SkinVertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, aOffsetof(SkinVertex, texture), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT animElements = ARRAYSIZE(animLayout);
	UINT numElements = ARRAYSIZE(layout);
	RETURN_ON_FAIL(SHADERS.addPair(DEVICE, CONTEXT,
		"Shader/Basic3D.fx", "Shader/Basic3D.fx",
		layout, numElements, "Basic3D"));

	RETURN_ON_FAIL(SHADERS.addPair(DEVICE, CONTEXT,
		"Shader/Basic2D.fx", "Shader/Basic2D.fx",
		layout, numElements, "Basic2D"));

	RETURN_ON_FAIL(SHADERS.addPair(DEVICE, CONTEXT,
		"Shader/BasicSky.fx", "Shader/BasicSky.fx",
		layout, numElements, "BasicSky"));

	RETURN_ON_FAIL(SHADERS.addPair(DEVICE, CONTEXT,
		"Shader/FireWork.fx", "Shader/FireWork.fx",
		layout,numElements,"FireWork"));

	RETURN_ON_FAIL(SHADERS.addPair(DEVICE, CONTEXT,
		"Shader/hblur/hblur.fx", "Shader/hblur/hblur.fx",
		layout, numElements, "Hblur"));

	RETURN_ON_FAIL(SHADERS.addPair(DEVICE, CONTEXT,
		"Shader/vblur/vblur.fx", "Shader/vblur/vblur.fx",
		layout, numElements, "Vblur"));

	RETURN_ON_FAIL(SHADERS.addPair(DEVICE, CONTEXT,
		"Shader/wires/line.fx", "Shader/wires/line.fx",
		layout, numElements, "BasicLine"));
	RETURN_ON_FAIL(SHADERS.addPair(DEVICE, CONTEXT,
		"Shader/depthMap/depthMap.fx", "Shader/depthMap/depthMap.fx",
		layout, numElements, "DepthMap"));
	RETURN_ON_FAIL(SHADERS.addPair(DEVICE, CONTEXT,
		"Shader/depthMap/lightDepthMap.fx", "Shader/depthMap/lightDepthMap.fx",
		layout, numElements, "LightDepthMap"));
	RETURN_ON_FAIL(SHADERS.addPair(DEVICE, CONTEXT,
		"Shader/texture/ProjectionTex.fx", "Shader/texture/ProjectionTex.fx",
		layout, numElements, "ProjectionTex"));
	RETURN_ON_FAIL(SHADERS.addPair(DEVICE, CONTEXT,
	"Shader/ShadowMap/ShadowMap.fx", "Shader/ShadowMap/ShadowMap.fx",
	layout, numElements, "ShadowMap"));
	RETURN_ON_FAIL(SHADERS.addPair(DEVICE, CONTEXT,
		"Shader/texture/AA/MSAA4x4.fx", "Shader/texture/AA/MSAA4x4.fx",
		layout, numElements, "MSAA4x4"));
	RETURN_ON_FAIL(SHADERS.addPair(DEVICE, CONTEXT,
		"Shader/HighLight/HighLight.fx", "Shader/HighLight/HighLight.fx",
		layout, numElements, "HighLight"));
	RETURN_ON_FAIL(SHADERS.addPair(DEVICE, CONTEXT,
		"Shader/Glow/GlowMerge.fx", "Shader/Glow/GlowMerge.fx",
		layout, numElements, "GlowMerge"));
	RETURN_ON_FAIL(SHADERS.addPair(DEVICE, CONTEXT,
		"Shader/DepthField/DepthFieldMerge.fx", "Shader/DepthField/DepthFieldMerge.fx",
		layout, numElements, "DepthFieldMerge"));
	RETURN_ON_FAIL(SHADERS.addPair(DEVICE, CONTEXT,
		"Shader/skinAnim/skinAnim.fx", "Shader/skinAnim/skinAnim.fx",
		animLayout, animElements, "SkinAnim"));

	scene.Initialize(g_hWnd, DEVICE, CONTEXT);
	return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
	FreeConsole();
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
	HDC hdc;
	RECT rc;
	UINT width; 
	UINT height;
    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

		case WM_KEYDOWN:
			INPUT.keys[wParam] = true;
			break;
		case WM_KEYUP:
			INPUT.keys[wParam] = false;
			break;
		case WM_LBUTTONDOWN:
			INPUT.keys[VK_LBUTTON] = true;
			break;
		case WM_LBUTTONUP:
			INPUT.keys[VK_LBUTTON] = false;
			break;
		case WM_RBUTTONDOWN:
			INPUT.keys[VK_RBUTTON] = true;
			break;
		case WM_RBUTTONUP:
			INPUT.keys[VK_RBUTTON] = false;
			break;
		case WM_MOUSEMOVE:
			INPUT.mouse_x = LOWORD(lParam);
			INPUT.mouse_y = HIWORD(lParam);
			break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

		case WM_SIZE:
			//Update window size
			GetClientRect(g_hWnd, &rc);
			width = rc.right - rc.left;
			height = rc.bottom - rc.top;

			WindowClass::getInstance().setWidth(width);
			WindowClass::getInstance().setHeight(height);
			WindowClass::getInstance().setResolutionWidth(width);
			WindowClass::getInstance().setResolutionHeight(height);
			break;
        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}

void CameraProc()
{
	scene.getCamera()->setStep(0.00015f);
	scene.getCamera()->moveForward(INPUT.keys['W']);
	scene.getCamera()->moveBackward(INPUT.keys['S']);
	scene.getCamera()->moveLeftward(INPUT.keys['A']);
	scene.getCamera()->moveRightward(INPUT.keys['D']);
	if (INPUT.keys[VK_LEFT])
	{
		scene.getCamera()->getCamera()->turnLeft(5);
	}
	if (INPUT.keys[VK_RIGHT])
	{
		scene.getCamera()->getCamera()->turnRight(5);
	}
	if (INPUT.keys[VK_UP])
	{
		scene.getCamera()->getCamera()->lookUp(2);
	}
	if (INPUT.keys[VK_DOWN])
	{
		scene.getCamera()->getCamera()->lookDown(2);
	}
}

void Render()
{
	CameraProc();
	d3d.clearRenderTarget();
	
	scene.Render(&d3d);

	d3d.Present(true);//V-Sync
}
