// dllmain.cpp : Defines the entry point for the DLL application.
#include <iostream>
#include <cstdio>
#include <cstdint>
#include "CJaffaVM.h"

#include "Process.h"

#include "MinHook.h"
#pragma comment(lib,"libMinHook.x86.lib")

HMODULE myhModule;
DWORD WINAPI EjectThread(LPVOID);
void Shutdown(FILE* fp, std::string reason);
DWORD WINAPI Menu();
void EventHandler();

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
    MH_STATUS status = MH_Initialize();
    if (status != MH_OK)
    {
		Shutdown(fp, "MinHook failed to initialize!");
	}
    EventHandler();
    Shutdown(fp, "Shutting down!");
    return 0;
}

void EventHandler()
{
    struct Process process;
    process.Init();
    while (1) {
        Sleep(100);
        if (GetAsyncKeyState(VK_NUMPAD1))
        {
            //DWORD addr_CJaffaVM = *(DWORD*)((DWORD)process.hmodule + 0x3164E8);
            //Thank you GitHub CoPilot.
            CJaffaVM sys_vm = *(CJaffaVM*)(*(DWORD*)((DWORD)process.hmodule + 0x3164E8));

            printf("%08X\n", sys_vm);
            printf("%08X\n", sys_vm.CPages);

            //DWORD CJaffaVM = *(DWORD*)((DWORD)process.hmodule + 0x3164E8);
            //printf("Address of CJaffaVM instance is %08X\n", CJaffaVM);
            //DWORD CPages = *(DWORD*)(CJaffaVM + 0x184);
            //printf("Address of CPages instance is %08X\n", CPages);
            //DWORD RANCE = *(DWORD*)(CPages + 0x13C0 * 4);
            //printf("Address of RANCE instance is %08X\n", RANCE);
            //DWORD Char_Data = *(DWORD*)(RANCE + 0x8);
            //printf("Address of Char_Data instance is %08X\n", Char_Data);
            //DWORD* p_HP = (DWORD*)(Char_Data + 0x40);
            //printf("Address of HP is %08X\n", p_HP);
            //*p_HP = 999999;
        }

        if (GetAsyncKeyState(VK_NUMPAD0))
            return;
    }
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