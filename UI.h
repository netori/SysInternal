#pragma once

#define WIN32_LEAN_AND_MEAN    
#include <windows.h>
#include <stdexcept>

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class UI
{
private:
	static ID3D11Device* pd3dDevice;
	static ID3D11DeviceContext* pd3dDeviceContext;
	static IDXGISwapChain* pSwapChain;
	static ID3D11RenderTargetView* pMainRenderTargetView;

	static HWND hWnd;
	static WNDPROC originalWndProc;
	static WNDCLASSEX windowClass;

	static constexpr float clear_color_with_alpha[4] = { 0.45f * 1.00f, 0.55f * 1.00f, 0.60 * 1.00f, 1.00f };

	static void SetupWindow(const wchar_t*);
	static void DestroyWindow();
	static void SetupImGui(HWND, ID3D11Device*, ID3D11DeviceContext*);
	static void DestroyImGui();
	static void CreateDeviceD3D(HWND);
	static void ReleaseDeviceD3D();
	static void CreateRenderTarget();
	static void CleanupRenderTarget();
	
	static LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

public:
	static bool isOpen;

	static void Setup();
	static void Cleanup();
	static void Render();

};
