// This header is the interface between GUI and the backend.
// Most functions are just wrappers for the backend.
#pragma once
#include "framework.h"
#include <cstdint>

#include "CPage.h"
#include "System4VM.h"

namespace Interface
{
	//static const wchar_t* moduleName = L"System4VM.dll";
	static const HMODULE moduleBase = GetModuleHandle(NULL);
	uintptr_t GetCJaffaVM();
	uintptr_t GetCPage(int);
	wchar_t* GetCPageName(int);
};