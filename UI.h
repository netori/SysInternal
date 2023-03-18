#pragma once

#include <iostream>
#include <stdexcept>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#include <d3d9types.h>



namespace UI
{
	inline bool open = true;
	inline bool setup = false;
	inline HWND hWnd  = nullptr;
	inline WNDCLASSEX windowClass = { };
	inline WNDPROC originalWndProc = nullptr;
	inline LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr;
	inline LPDIRECT3D9 d3d9 = nullptr;
	inline ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	inline D3DPRESENT_PARAMETERS d3dpp = { };

	
	void Setup();
	void SetupWindow(const wchar_t* windowClassName);
	void DestroyWindowClass();
	void DestroyWindow();
	void SetupD3D();
	void ReleaseD3D();
	void CreateRenderTarget();
	void ResetDevice();
	void SetupImGui(LPDIRECT3DDEVICE9);
	void DestroyImGui();
	void Render();
}