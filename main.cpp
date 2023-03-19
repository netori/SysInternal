#include "UI.h"

DWORD WINAPI Init();

#ifdef _WINDLL
HMODULE myhModule;
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        myhModule = hModule;
        CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Init), NULL, NULL, NULL);
        break;
    }
    return TRUE;
}
#else
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    Init();
}
#endif

DWORD WINAPI Init()
{
    UI::Setup();
    UI::Render();
    UI::Cleanup();
#ifdef _WINDLL
    FreeLibraryAndExitThread(myhModule, NULL);
#endif
    return 0;
}

/*
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
*/