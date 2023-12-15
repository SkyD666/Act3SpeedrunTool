#pragma once
#include <windows.h>

class MemoryUtil {
public:
    MemoryUtil();

    static HWND getWindowHwnd();

    static HANDLE getProcessHandle(DWORD* pid, DWORD dwDesiredAccess = PROCESS_ALL_ACCESS);

    static HMODULE getProcessModuleHandle(DWORD pid, CONST TCHAR* moduleName);
};
