// dllmain.cpp : Defines the entry point for the DLL application.
#include <iostream>
#include "framework.h"

#include "MinHook.h"
#pragma comment(lib,"libMinHook.x86.lib")

HMODULE myhModule;
DWORD WINAPI EjectThread(LPVOID);
void Shutdown(FILE* fp, std::string reason);
DWORD WINAPI Menu();
void Hook();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        myhModule = hModule;
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Menu, NULL, NULL, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

DWORD WINAPI Menu()
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    std::cout << "Dll Injected at 0x" << &myhModule << std::endl;
    std::cout << "RanceIX == " << GetModuleHandle(NULL) << std::endl;

    MH_STATUS status = MH_Initialize();
    if (status != MH_OK)
    {
		Shutdown(fp, "MinHook failed to initialize!");
	}

    while (1) {
        Sleep(1000);
        if (GetAsyncKeyState(VK_NUMPAD1))
        {
            std::cout << "Hooking!" << std::endl;
            Hook();
        }
            
        if (GetAsyncKeyState(VK_NUMPAD0))
            break;
    }
    Shutdown(fp, "Shutting down!");
    return 0;
}

void Hook()
{

}

void Shutdown(FILE* fp, std::string reason)
{
    //Output shutdown reason
    std::cout << reason << std::endl;
    system("pause");
    //Cleanup
    fclose(fp);
    FreeConsole();
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)EjectThread, NULL, NULL, NULL);
}


DWORD __stdcall EjectThread(LPVOID lpParameter)
{
    Sleep(100);
    FreeLibraryAndExitThread(myhModule, NULL);
}