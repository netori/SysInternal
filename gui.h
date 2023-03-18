#pragma once
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

namespace UI
{
	inline bool open = true;
	inline bool setup = false;
	inline HWND window = nullptr;
	inline WNDCLASSEX windowClass = { };
	inline WNDPROC originalWndProc = nullptr;
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline LPDIRECT3D9 d3d9 = nullptr;

	void Setup();
	bool SetupWindowClass(const wchar_t* windowClassName);
	void DestroyWindowClass();
	bool SetupWindow(const wchar_t* windowClassName, const wchar_t* windowName);
	void DestroyWindow();
	bool SetupD3D();
	void DestroyD3D();
	void ResetDevice();
	void SetupImGui(LPDIRECT3DDEVICE9);
	void DestroyImGui();
	void Render();
}