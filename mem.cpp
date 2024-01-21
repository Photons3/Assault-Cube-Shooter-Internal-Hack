#include "pch.h"
#include "mem.h"


void mem::PatchEx(BYTE* dst, BYTE* src, UINT size, HANDLE hProcess)
{
	DWORD oldProtect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	WriteProcessMemory(hProcess, dst, src, size, NULL); 
	VirtualProtectEx(hProcess, dst, size, oldProtect, &oldProtect);
}

void mem::NopEx(BYTE* dst, UINT size, HANDLE  hProcess)
{
	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size);

	DWORD oldProtect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	PatchEx(dst, nopArray, size, hProcess);
	VirtualProtectEx(hProcess, dst, size, oldProtect, &oldProtect);

	delete[] nopArray;
}

void mem::Patch(BYTE* dst, BYTE* src, UINT size)
{
	DWORD oldProtect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldProtect, &oldProtect);
}

void mem::Nop(BYTE* dst, UINT size)
{

	DWORD oldProtect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(dst, 0x90, size);
	VirtualProtect( dst, size, oldProtect, &oldProtect);
}

UINT mem::FindDMAAddy( uintptr_t  ptrBaseAddr, std::vector<unsigned int> offsets)
{
	UINT addr = ptrBaseAddr;
	for (UINT i = 0; i < offsets.size(); ++i)
	{
		addr = *(UINT*)(addr);
		addr += offsets[i];
	}
	return addr;
}

BOOL mem::Detour32(BYTE* src, BYTE* dst, UINT length) 
{
	if (length < 5) return false;

	DWORD oldProtect;
	VirtualProtect(dst, length, PAGE_EXECUTE_READWRITE, &oldProtect);

	UINT relativeAddr = (UINT)(dst - src - 5);
	*src = 0xE9;
	*(UINT*)(src + 1) = relativeAddr;

	VirtualProtect(dst, length, oldProtect, &oldProtect);

	return true;
}

BYTE* mem::TrampHook32(BYTE* src, BYTE* dst, const uintptr_t length)
{
	if (length < 5) return 0;

	DWORD oldProtect;
	VirtualProtect(src, length, PAGE_EXECUTE_READWRITE, &oldProtect);

	BYTE* gateway = (BYTE*)VirtualAlloc(0, length, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy_s(gateway, length, src, length);

	UINT relativeGatewayAddr = src - gateway - 5;
	*(gateway + length) = 0xE9;
	*(uintptr_t*)(uintptr_t)(gateway + length + 1) = relativeGatewayAddr;

	Detour32(src, dst, length);
	
	return gateway;
}

