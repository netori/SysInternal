#include "gui.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

#include <stdexcept>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void UI::Setup()
{
	if (!UI::SetupWindowClass(L"ImGuiWindowClass"))
	{
		throw std::runtime_error("Failed to create window class");
	}
	if (!UI::SetupWindow(L"ImGuiWindowClass", L"ImGui"))
	{
		throw std::runtime_error("Failed to create window");
	}
	if (!UI::SetupD3D())
	{
		throw std::runtime_error("Failed to create d3d9 device");
	}
	//Destroy the original window procedure
	UI::DestroyWindow();
	UI::DestroyWindowClass();

	//Create the ImGui context
	ImGui::CreateContext();
	//Setup the ImGui style
	ImGui::StyleColorsDark();
	//Setup the ImGui platform
	ImGui_ImplWin32_Init(window);
	//Setup the ImGui renderer
	ImGui_ImplDX9_Init(device);
}

bool UI::SetupWindowClass(const wchar_t* windowClassName)
{
	//Set up the window class.
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
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
		return false;
	}
	return true;
}

void UI::DestroyWindowClass()
{
	UnregisterClass(reinterpret_cast<LPCWSTR>(windowClass.lpszClassName), windowClass.hInstance);
}

////https://learn.microsoft.com/en-us/windows/win32/learnwin32/creating-a-window
bool UI::SetupWindow(const wchar_t* windowClassName, const wchar_t* windowName)
{
	window = CreateWindow(windowClass.lpszClassName, windowName, WS_OVERLAPPEDWINDOW, NULL, NULL, 100, 100, NULL, NULL, windowClass.hInstance, NULL);
	if (!window)
	{
		return false;
	}
	return true;
}

void UI::DestroyWindow()
{
	if (window)
	{
		DestroyWindow(window);
		window = nullptr;
	}
}

//https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-direct3dcreate9
bool UI::SetupD3D()
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
	if ((d3d9 = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		return false;
	}
	//These are the parameters we will use to create the IDirect3DDevice9 object
	D3DPRESENT_PARAMETERS d3dpp = { };
	d3dpp.BackBufferWidth = 0;
	d3dpp.BackBufferHeight = 0;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 0;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = NULL;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = window;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = FALSE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (SUCCEEDED(d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		window, 
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, 
		&d3dpp, 
		&device
	)))
	{
		return true;
	}

	return false;
}

void UI::DestroyD3D()
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}
	if (d3d9)
	{
		d3d9->Release();
		d3d9 = nullptr;
	}
}

void UI::ResetDevice()
{

}




void UI::SetupImGui(LPDIRECT3DDEVICE9 device)
{
	//
	auto params = D3DDEVICE_CREATION_PARAMETERS{};
	device->GetCreationParameters(&params);
	//Get the window handle
	window = params.hFocusWindow;
	UI::originalWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));

	//Create the ImGui context
	ImGui::CreateContext();
	//Setup the ImGui style
	ImGui::StyleColorsDark();
	//Setup the ImGui platform
	ImGui_ImplWin32_Init(window);
	//Setup the ImGui renderer
	ImGui_ImplDX9_Init(device);
	UI::Render();
	setup = true;
}

void UI::DestroyImGui()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	//Restore the original window procedure
	SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(UI::originalWndProc));
	DestroyD3D();
}

void UI::Render()
{
	if (!setup)
	{
		return;
	}
	//Start the ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//Render the menu
	if (UI::open)
	{
		ImGui::Begin("ImGui", &UI::open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::Text("Hello, world!");
		ImGui::End();
	}
	//Render the ImGui frame
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Toggle menu
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		MessageBoxA(NULL,"A","A",MB_OK);
		UI::open = !UI::open;
	}
	//If the menu is open, handle input for IMGUI instead of the original window.
	if (UI::open && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}

	//Call original window procedure
	return CallWindowProc(UI::originalWndProc, hWnd, msg, wParam, lParam);
}