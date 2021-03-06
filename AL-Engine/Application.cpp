#include "Core.h"
#include "Application.h"

namespace AL
{
	HRESULT CALLBACK OnD3D9CreateDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext)
	{
		Director::Init();
		SoundManager::GetInstance()->Init();
		return S_OK;
	}

	void CALLBACK OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext)
	{
		HRESULT hr;

		V(pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0));

		if (SUCCEEDED(pd3dDevice->BeginScene()))
		{
			Time::deltaTime = fElapsedTime * Time::timeScale;
			Time::time += Time::deltaTime;

			Director::GetInstance()->Update();

			V(pd3dDevice->EndScene());
		}
	}

	void CALLBACK OnD3D9DestroyDevice(void* pUserContext)
	{
		TextureManager::GetInstance()->Drain();
		SoundManager::GetInstance()->ReleaseSystem();
		exit(0);
	}

	Application::Application() :
		width(1280),
		height(720),
		isWindow(true),
		title(L"AL-Engine")
	{ }

	Application::~Application()
	{ }

	void Application::Init(Scene * scene)
	{
		DXUTSetCallbackD3D9DeviceCreated(OnD3D9CreateDevice);
		DXUTSetCallbackD3D9FrameRender(OnD3D9FrameRender);
		DXUTSetCallbackD3D9DeviceDestroyed(OnD3D9DestroyDevice);

		DXUTInit(true, true); // Parse the command line and show msgboxes
		DXUTSetHotkeyHandling(true, true, true);  // handle the default hotkeys
		DXUTSetCursorSettings(true, true); // Show the cursor and clip it when in full screen
		DXUTCreateWindow(title.c_str());
		DXUTCreateDevice(isWindow, width, height);

		Director::GetInstance()->ChangeScene(scene);
	}

	int Application::Start()
	{
		DXUTMainLoop();
		return DXUTGetExitCode();
	}
}
