// This header is the interface between GUI and the backend.
#pragma once
#define WIN32_LEAN_AND_MEAN    
#include <Windows.h>
#include <cstdint>

#include "CPage.h"
//#include "System4VM.h"

namespace Interface
{
	static const HMODULE moduleBase = GetModuleHandle(NULL); 
	DWORD GetCJaffaVM();

	DWORD GetCPage(int);

}