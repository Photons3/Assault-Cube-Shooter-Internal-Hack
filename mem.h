#pragma once
#include "pch.h"

namespace mem
{
	//External
	void PatchEx(BYTE* dst, BYTE* src, UINT size, HANDLE hProcess);
	void NopEx(BYTE* dst, UINT size, HANDLE  hProcess);

	//Internal
	void Patch(BYTE* dst, BYTE* src, UINT size);
	void Nop(BYTE* dst, UINT size);
	UINT FindDMAAddy(uintptr_t  ptrBaseAddr, std::vector<unsigned int> offsets);
	BOOL Detour32(BYTE* src, BYTE* dst, UINT length);
	BYTE* TrampHook32(BYTE* src, BYTE* dst, const uintptr_t length);
}

