#include "pch.h"
#include "mem.h"
#include "proc.h"
#include "Reclass.h"

UINT moduleBase = (UINT)GetModuleHandle(L"ac_client.exe");
bool bHealth = false, bRecoil = false, bAmmo = false, bTrigger = false;

typedef BOOL(_stdcall* twglSwapBuffers)(HDC hDc);
typedef ent* (__cdecl* tGetCrossHairEnt)();

twglSwapBuffers owglSwapBuffers;
tGetCrossHairEnt GetCrossHairEnt = nullptr;

BOOL _stdcall hkSwapBuffers(HDC hDc)
{
    ent* localPlayerPtr = *(ent**)(moduleBase + 0x10f4f4);
    GetCrossHairEnt = (tGetCrossHairEnt)(moduleBase + 0x607c0);

    if (GetAsyncKeyState(VK_NUMPAD1) & 1)
    {
        bHealth = !bHealth;
    }
    if (GetAsyncKeyState(VK_NUMPAD2) & 1)
    {
        bAmmo = !bAmmo;
    }
    if (GetAsyncKeyState(VK_NUMPAD3) & 1)
    {
        bRecoil = !bRecoil;
        if (bRecoil)
        {
            mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
        }
        else
        {
            mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
        }
    }
    if (GetAsyncKeyState(VK_NUMPAD4) & 1)
    {
        bTrigger = !bTrigger;
    }

    if (localPlayerPtr)
    {
        if (bHealth)
        {
            localPlayerPtr->health = 1337;
        }

        if (bAmmo)
        {
            localPlayerPtr->ammoWeap = 1337;

        }

        if (bTrigger)
        {
            ent* crosshairEnt = GetCrossHairEnt();  

            if (crosshairEnt)
            {
                if (localPlayerPtr->teamId != crosshairEnt->teamId)
                {
                    localPlayerPtr->isShooting = 1;
                }
                else
                {
                    localPlayerPtr->isShooting = 0;
                }
            }
                
        }
    }
       
    return owglSwapBuffers(hDc);
}
DWORD WINAPI HackThread(HMODULE hModule)
{
   
    owglSwapBuffers = (twglSwapBuffers)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
    owglSwapBuffers = (twglSwapBuffers)mem::TrampHook32((BYTE*)owglSwapBuffers, (BYTE*)hkSwapBuffers, 5);

    return 0;

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) HackThread, hModule, 0, NULL);
        if (hThread)
        {
            CloseHandle(hThread);
        }

    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

