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
            //Thank you GitHub CoPilot.
            //Get the address of the CJaffaVM object.
            DWORD addr_CJaffaVM = *(DWORD*)((DWORD)process.hmodule + 0x3164E8);
            //Cast the address as a pointer to a CJaffaVM object and dereference it.
            CJaffaVM sys_vm = *(CJaffaVM*)(addr_CJaffaVM);

            Character* RANCE = (Character*)(sys_vm.cpage_arr->arr[5056]->Value); //5056 should be an enum
            printf("Rance's Character Data is at %08X\n", RANCE);
            printf("Rance's HP is %d\n", RANCE->Cur_HP);
            printf("Setting Rance's HP to 999999\n");
            RANCE->Cur_HP = 999999;
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