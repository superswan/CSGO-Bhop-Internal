// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

using namespace hazedumper;

DWORD WINAPI HackThread(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "CSGO BHOP v0.1\n";

    //Get module
    uintptr_t gameModuleBaseAddr = (uintptr_t)GetModuleHandle(L"client.dll");
    DWORD localPlayer = *(DWORD*)(gameModuleBaseAddr + signatures::dwLocalPlayer);

    std::cout << "Base address for client.dll: " << std::hex << gameModuleBaseAddr << std::endl;
    
    //Loop until LocalPlayer address is accessible
    if (localPlayer == NULL)
        while (localPlayer == NULL)
        {
            uintptr_t* localPlayer = (uintptr_t*)(gameModuleBaseAddr + signatures::dwLocalPlayer);
        }
    std::cout << "Local Player Address: " << std::hex << localPlayer << std::endl;

    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        BYTE flagValue = *(BYTE*)(localPlayer + netvars::m_fFlags);
        if ((GetAsyncKeyState(VK_SPACE) & 1) && (flagValue & 1))
        {
            *(DWORD*)(gameModuleBaseAddr + signatures::dwForceJump) = 6;
        }

    }

    //cleanup & eject
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

