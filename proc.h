#pragma once

#include "pch.h"


DWORD GetProcId(const wchar_t* procName);

UINT GetModuleBaseAddress(DWORD procId, const wchar_t* modName);

UINT FindDMAAddyEx(HANDLE hProc, uintptr_t  ptrBaseAddr, std::vector<unsigned int> offsets);


