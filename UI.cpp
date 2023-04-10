#include "UI.h"
#include "System4Editor.h"

ID3D11Device* UI::pd3dDevice = nullptr;
ID3D11DeviceContext* UI::pd3dDeviceContext = nullptr;
IDXGISwapChain* UI::pSwapChain = nullptr;
ID3D11RenderTargetView* UI::pMainRenderTargetView = nullptr;

HWND UI::hWnd = nullptr;
WNDCLASSEX UI::windowClass = { 0 };
bool UI::isOpen = true;


void UI::Setup()
{
	try
	{
		UI::SetupWindow(L"SysInternal");
		UI::CreateDeviceD3D(UI::hWnd);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Fatal Error", MB_OK);
		abort();
	}
	UI::CreateRenderTarget();
	UI::SetupImGui(UI::hWnd, UI::pd3dDevice, UI::pd3dDeviceContext);
}

////https://learn.microsoft.com/en-us/windows/win32/learnwin32/creating-a-window
void UI::SetupWindow(const wchar_t* windowClassName)
{
	ImGui_ImplWin32_EnableDpiAwareness();
	//Erase any garbage data.
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	//Set up the window class to be registered.
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0L;
	windowClass.cbWndExtra = 0L;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = nullptr;
	windowClass.hCursor = nullptr;
	windowClass.hbrBackground = nullptr;
	windowClass.lpszMenuName = nullptr;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = nullptr;
	if (!RegisterClassEx(&windowClass))
	{
		throw std::runtime_error("Failed to register window class");
	}
	UI::hWnd = ::CreateWindow(windowClass.lpszClassName, windowClassName, WS_OVERLAPPEDWINDOW, 100, 100, 50, 50, NULL, NULL, windowClass.hInstance, NULL);
	if (!UI::hWnd)
	{
		throw std::runtime_error("Failed to create window");
	}
	::ShowWindow(UI::hWnd, SW_HIDE);
	::UpdateWindow(UI::hWnd);
	return;
}

void UI::DestroyWindow()
{
	::DestroyWindow(UI::hWnd);
	UnregisterClass(UI::windowClass.lpszClassName, windowClass.hInstance);
}

void UI::CreateDeviceD3D(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd;
	// Clean up the memory of the swap chain description.
	ZeroMemory(&sd, sizeof(sd));
	// Set the swap chain description.
	sd.BufferCount = 2;									// Use double buffering.
	sd.BufferDesc.Width = 0;							// Set the width of the back buffer to the width of the window.
	sd.BufferDesc.Height = 0;							// Set the height of the back buffer to the height of the window.
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// Set the format of the back buffer.
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;  // Allow the swap chain to switch to fullscreen mode.
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// Set the usage of the back buffer.
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;							// 1 sample per pixel.		
	sd.SampleDesc.Quality = 0;							// No multisampling.
	sd.Windowed = TRUE;									// Start in windowed mode.
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;	// Use the flip model for swap chains due to efficiency.

	const UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel = { };
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,				   // Use the default adapter.
		D3D_DRIVER_TYPE_HARDWARE,  // Use the hardware graphics driver.
		nullptr, 				   // Do not use external software rasterizer.
		createDeviceFlags, 		   // Do not use debug layer.
		featureLevelArray, 		   // Use the feature level array.
		2,						   // Number of feature levels in the array.
		D3D11_SDK_VERSION, 		   // Use the latest SDK version.
		&sd, 					   // Use the swap chain description.
		&pSwapChain, 
		&pd3dDevice, 
		&featureLevel, 
		&pd3dDeviceContext
	);

	if (hr != S_OK)
	{
		throw std::runtime_error("Failed to create device and swap chain");
	}
	return;
}

void UI::CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (pBackBuffer != nullptr)
	{
		pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pMainRenderTargetView);
		pBackBuffer->Release();
	}
}

void UI::CleanupRenderTarget()
{
	if (pMainRenderTargetView)
	{
		pMainRenderTargetView->Release();
		pMainRenderTargetView = nullptr;
	}
}

void UI::ReleaseDeviceD3D()
{
	CleanupRenderTarget();
	if (pSwapChain)
	{
		pSwapChain->Release();
		pSwapChain = nullptr;
	}

	if (pd3dDeviceContext)
	{
		pd3dDeviceContext->Release();
		pd3dDeviceContext = nullptr;
	}

	if (pd3dDevice)
	{
		pd3dDevice->Release();
		pd3dDevice = nullptr;
	}
}

void UI::SetupImGui(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* devicecontext)
{
	IMGUI_CHECKVERSION();
	// Create the ImGui context
	ImGui::CreateContext();
	// Enable mult-viewport
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	// Setup the ImGui style
	ImGui::StyleColorsClassic();
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 4.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	// Setup the monitor height
	const HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO info = {};
	info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &info);
	const int monitor_height = info.rcMonitor.bottom - info.rcMonitor.top;

	if (monitor_height > 1080)
	{
		const float fScale = 2.0f;
		ImFontConfig cfg;
		cfg.SizePixels = 13 * fScale;
		ImGui::GetIO().Fonts->AddFontDefault(&cfg);
	}
	else
	{
		ImGui::GetIO().Fonts->AddFontDefault();
	}
	ImGui::GetIO().IniFilename = nullptr;
	// Setup the ImGui platform
	ImGui_ImplWin32_Init(hwnd);
	// Setup the ImGui renderer
	ImGui_ImplDX11_Init(device, devicecontext);

}

void UI::DestroyImGui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void UI::Render()
{
	MSG msg;
	while (UI::isOpen)
	{
		// Check for messages.
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				UI::isOpen = false;
		}
		// Start the ImGui frame.
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		// Draw the ImGuiFrame to backbuffer.
		{
			//ImGui::ShowDemoWindow();
			if (!System4Editor::Render())
				UI::isOpen = false;
		}
		// End the ImGui frame.
		ImGui::EndFrame();
		// Render the ImGui frame.
		ImGui::Render();
		pd3dDeviceContext->OMSetRenderTargets(1, &pMainRenderTargetView, nullptr);
		pd3dDeviceContext->ClearRenderTargetView(pMainRenderTargetView, UI::clear_color_with_alpha);
		// Actually draw the ImGui frame.
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		// Viewport rendering
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
		// Present the shit.
		pSwapChain->Present(1, 0);
	}
}

void UI::Cleanup()
{
	DestroyImGui();
	ReleaseDeviceD3D();
	DestroyWindow();
}

// WndProc is our window procedure; it handles all the messages sent to our window then calls the default window procedure.
// However, we also need to call ImGui_ImplWin32_WndProcHandler to allow ImGui to capture our inputs to the imgui window.
LRESULT WINAPI UI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// If the menu is open, handle input for IMGUI instead of the original window.
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
		// Post a quit message (WM_QUIT) to the message queue.
		::PostQuitMessage(0);
		return 0;

	default:
		break;
	}
	// Call the default window procedure.
	// Notice that the namespace operator (::) is used to call the DefWindowProc function within the global namespace.
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}