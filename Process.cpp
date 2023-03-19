#include "Process.h"


#include "MinHook.h"
#pragma comment(lib,"libMinHook.x86.lib")


void Process::Init()
{
	hmodule = GetModuleHandle(NULL);
}