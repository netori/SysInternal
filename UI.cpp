#include "UI.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void UI::Setup()
{
	try
	{
		//
		UI::SetupWindow(L"SysInternal");
		UI::SetupD3D();
		ImGui_ImplWin32_EnableDpiAwareness();
		UI::CreateRenderTarget();
		::ShowWindow(UI::hWnd, SW_SHOWDEFAULT);
		::UpdateWindow(UI::hWnd);
		UI::SetupImGui(UI::g_pd3dDevice);
		//UI::ReleaseD3D();
		//UI::DestroyWindow();
		//SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(UI::originalWndProc));
	}
	catch (const std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Fatal Error", MB_OK);
		abort();
	}
}

////https://learn.microsoft.com/en-us/windows/win32/learnwin32/creating-a-window
void UI::SetupWindow(const wchar_t* windowClassName)
{
	//Erase any garbage data.
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	//Set up the window class to be registered.
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0L;
	windowClass.cbWndExtra = 0L;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = NULL;
	//If the window class fails to register, return false.
	if (!RegisterClassEx(&windowClass))
	{
		throw std::runtime_error("Failed to register window class");
	}
	//Create the window.
	hWnd = ::CreateWindow(windowClass.lpszClassName, windowClassName, WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, windowClass.hInstance, NULL);
	if (!hWnd)
	{
		throw std::runtime_error("Failed to create window");
	}
	return;
}

void UI::DestroyWindow()
{
	UnregisterClass(reinterpret_cast<LPCWSTR>(windowClass.lpszClassName), windowClass.hInstance);
	::DestroyWindow(hWnd);
}

//https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-direct3dcreate9
void UI::SetupD3D()
{
	/* Only for hooking
	//Get a handle to the d3d9.dll
	const auto handle = GetModuleHandle(L"d3d9.dll");
	if (!handle)
	{
		return false;
	}
	
	//Get the export address of the Direct3DCreate9 function
	using funcType = LPDIRECT3D9(__stdcall*)(UINT);
	const auto direct3DCreate9 = reinterpret_cast<funcType>(GetProcAddress(handle, "Direct3DCreate9"));
	//Create the IDirect3D9 object
	d3d9 = direct3DCreate9(D3D_SDK_VERSION);
	*/
	//Create the IDirect3D9 object
	if ((d3d9 = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		throw std::runtime_error("Failed to create IDirect3D9 object");
	}

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
	
	////These are the parameters we will use to create the IDirect3DDevice9 object.
	//d3dpp.BackBufferWidth = 0;					   //Set to 0 to use the width of the desktop.
	//d3dpp.BackBufferHeight = 0;					   //Set to 0 to use the height of the desktop.
	//d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	   //Set to D3DFMT_UNKNOWN to use the desktop's current format.
	//d3dpp.BackBufferCount = 1;					   //Use double buffering.
	//d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;   //No multisampling since we are not using D3DSWAPEFFECT_DISCARD.
	//d3dpp.MultiSampleQuality = NULL;			   //No multisampling since we are not using D3DSWAPEFFECT_DISCARD.
	//d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		   //Moderns computers can afford the overhead.		
	//d3dpp.hDeviceWindow = UI::hWnd;
	//d3dpp.Windowed = TRUE;
	//d3dpp.EnableAutoDepthStencil = FALSE;		  //No depth buffer.
	//d3dpp.AutoDepthStencilFormat = D3DFMT_UNKNOWN; //No depth buffer.
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; //Present with no vsync.
	//d3dpp.Flags = NULL;							  //No flags.

	//The call to IDirect3D9::CreateDevice then creates the Direct3D device and corresponding swap chain.
	if (FAILED(d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		UI::hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&d3dpp,
		&g_pd3dDevice
	)))
	{
		throw std::runtime_error("Failed to create device");
	}
	return;
}

void UI::ReleaseD3D()
{
	if (g_pd3dDevice)
	{
		g_pd3dDevice->Release();
		g_pd3dDevice = nullptr;
	}
	if (d3d9)
	{
		d3d9->Release();
		d3d9 = nullptr;
	}
}

void UI::ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = UI::g_pd3dDevice->Reset(&UI::d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

void UI::CreateRenderTarget()
{
	UINT a = g_pd3dDevice->GetNumberOfSwapChains();
	IDirect3DSurface9* backBuffer = nullptr;
	g_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	if (backBuffer == nullptr)
	{
		throw std::runtime_error("Failed to get back buffer");
	}
	g_pd3dDevice->CreateRenderTarget(1920, 1080, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0, TRUE, &backBuffer, NULL);
	//Release the back buffer
	backBuffer->Release();
}


void UI::SetupImGui(LPDIRECT3DDEVICE9 device)
{
	IMGUI_CHECKVERSION();
	//Create the ImGui context
	ImGui::CreateContext();
	//Setup the ImGui style
	ImGui::StyleColorsDark();
	//Setup the ImGui platform
	ImGui_ImplWin32_Init(UI::hWnd);
	//Setup the ImGui renderer
	ImGui_ImplDX9_Init(device);

	setup = true;
}

void UI::DestroyImGui()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	//Restore the original window procedure
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(UI::originalWndProc));
}

void UI::Render()
{
	//Start the ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//Render the menu
	{
		ImGui::ShowDemoWindow();
		//ImGui::Begin("ImGui", &UI::open, ImGuiWindowFlags_NoCollapse);
		//ImGui::Text("Hello, world!");
		//ImGui::End();
	}
	//Render the ImGui frame
	ImGui::EndFrame();
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
	if (g_pd3dDevice->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		g_pd3dDevice->EndScene();
	}
	HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	
	if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		UI::ResetDevice();
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//If the menu is open, handle input for IMGUI instead of the original window.
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}
	// Message handler
	switch (msg)
	{
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;

	case WM_DESTROY:
		//Post a quit message (WM_QUIT) to the message queue.
		::PostQuitMessage(0);
		return 0;

	default:
		break;
	}
	//Call the default window procedure.
	//Notice that the namespace operator (::) is used to call the DefWindowProc function within the global namespace.
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}