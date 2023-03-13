#include "Process.h"


void Process::Init()
{
	hmodule = GetModuleHandle(NULL);
}