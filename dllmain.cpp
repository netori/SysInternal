// dllmain.cpp : Defines the entry point for the DLL application.
#include <iostream>
#include <cstdio>
#include <cstdint>
#include "CJaffaVM.h"

#include "Process.h"
#include "gui.h"

#include "MinHook.h"
#pragma comment(lib,"libMinHook.x86.lib")

//typedef long(__stdcall* present)(IDXGISwapChain*, UINT, UINT);

HMODULE myhModule;
DWORD WINAPI EjectThread(LPVOID);
void Shutdown(FILE* fp, std::string reason);
DWORD WINAPI Menu();
void EventHandler();


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        myhModule = hModule;
        CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Menu), NULL, NULL, NULL);
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
    try
    {
        UI::Setup();

    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
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
            //Get the address of the array of pointers to CPage objects.
            Character RANCE = sys_vm.cpage_arr->getCharacter(CPageIndex::CHR_KANAMI_KENTOU);
            printf("Rance's Character Data is at %08X\n", RANCE);
            printf("Rance's HP is %d\n", RANCE.Cur_HP);
        } 

        if (GetAsyncKeyState(VK_NUMPAD0))
        {
            break;
        }
    }
    return;
}

void Shutdown(FILE* fp, std::string reason)
{
    UI::DestroyImGui();
    //Output shutdown reason
    std::cout << reason << std::endl;
    //system("pause");
    //Cleanup
    fclose(fp);
    FreeConsole();
    FreeLibraryAndExitThread(myhModule, NULL);
}