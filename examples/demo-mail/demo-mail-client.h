#pragma once

#include <windows.h>

// Force-include only while compiling SendEmail.cpp in the isolated mail test.
// The normal build calls Windows Simple MAPI and uses the user's mail client.
inline HMODULE WINAPI LoadDemoMailClient(LPCSTR name)
{
    wchar_t directory[MAX_PATH];
    const DWORD length = GetEnvironmentVariableW(L"XCRASHREPORT_DEMO_DIR", directory, MAX_PATH);
    const wchar_t library[] = L"\\XCrashReportDemo.dll";
    if (lstrcmpiA(name, "MAPI32.DLL") != 0 || length < 3 ||
        length + _countof(library) > MAX_PATH || directory[1] != L':' || directory[2] != L'\\') {
        return nullptr;
    }
    lstrcatW(directory, library);
    return LoadLibraryExW(directory, nullptr, LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_SYSTEM32);
}

#define LoadLibraryA LoadDemoMailClient
