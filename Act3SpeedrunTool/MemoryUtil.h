#pragma once
#include <windows.h>

class MemoryUtil {
public:
    MemoryUtil();

    static HANDLE getProcessHandle(DWORD* pid);

    static HMODULE getProcessModuleHandle(DWORD pid, CONST TCHAR* moduleName);
};
