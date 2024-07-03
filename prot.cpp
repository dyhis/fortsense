#include "prot.h"
#include <iphlpapi.h>
#include <winhttp.h>
#include <TlHelp32.h>
#include <wininet.h>

#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")  // Link with the IP Helper library
#pragma comment(lib, "winhttp.lib")  // Link with the IP Helper library


using namespace std;

bool IsWindowTitle(const wchar_t* windowTitle)
{
    HWND hwnd = FindWindow(NULL, (LPCSTR)windowTitle);
    return hwnd != NULL;
}

VOID WindowClassNames()
{
    vector<wstring> windowTitlesToClose = { L"Cheat Engine 7.5", L"UD", L"IDA", L"x64dbg", L"FileGrab", L"Nigger", L"Beammer", L"Process Hacker", L"dexzunpacker" };
    for (const auto& title : windowTitlesToClose)
    {
        if (IsWindowTitle(title.c_str()))
        {
            HWND hwnd = FindWindow(NULL, (LPCSTR)title.c_str());
            exit(0);
        }
    }
}

VOID SearchWindowClassNames()
{
    if (IsValid)
    {
        VL_OBFUSCATION_BEGIN;
        while (true) {
            WindowClassNames();
            Sleep(1);
        }
        VL_OBFUSCATION_END;
    }
}

BOOL MemoryBreakpoints_PageGuard()
{
    UCHAR* pMem = NULL;
    SYSTEM_INFO SystemInfo = { 0 };
    DWORD OldProtect = 0;
    PVOID pAllocation = NULL; // Get the page size for the system 

    // Retrieves information about the current system.
    GetSystemInfo(&SystemInfo);

    // Allocate memory 
    pAllocation = VirtualAlloc(NULL, SystemInfo.dwPageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (pAllocation == NULL)
        return FALSE;

    // Write a ret to the buffer (opcode 0xc3)
    RtlFillMemory(pAllocation, 1, 0xC3);

    // Make the page a guard page         
    if (VirtualProtect(pAllocation, SystemInfo.dwPageSize, PAGE_EXECUTE_READWRITE | PAGE_GUARD, &OldProtect) == 0)
        return FALSE;

    __try
    {
        ((void(*)())pAllocation)(); // Exception or execution, which shall it be :D?
    }
    __except (GetExceptionCode() == STATUS_GUARD_PAGE_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
    {
        VirtualFree(pAllocation, 0, MEM_RELEASE);
        return FALSE;
    }

    VirtualFree(pAllocation, 0, MEM_RELEASE);
    exit(0);
}


BOOL HardwareBreakpoints()
{
    // This structure is key to the function and is the 
    // medium for detection and removal
    PCONTEXT ctx = PCONTEXT(VirtualAlloc(NULL, sizeof(CONTEXT), MEM_COMMIT, PAGE_READWRITE));

    if (ctx) {

        SecureZeroMemory(ctx, sizeof(CONTEXT));

        // The CONTEXT structure is an in/out parameter therefore we have
        // to set the flags so Get/SetThreadContext knows what to set or get.
        ctx->ContextFlags = CONTEXT_DEBUG_REGISTERS;

        // Get the registers
        if (GetThreadContext(GetCurrentThread(), ctx)) {

            // Now we can check for hardware breakpoints, its not 
            // necessary to check Dr6 and Dr7, however feel free to
            if (ctx->Dr0 != 0 || ctx->Dr1 != 0 || ctx->Dr2 != 0 || ctx->Dr3 != 0)
                exit(0);
        }

        VirtualFree(ctx, 0, MEM_RELEASE);
    }
}



VOID AntiAttach() {
    HANDLE hProcess = GetCurrentProcess();
    HMODULE hMod = GetModuleHandleW(L"ntdll.dll");
    FARPROC func_DbgUiRemoteBreakin = GetProcAddress(hMod, "DbgUiRemoteBreakin");
    WriteProcessMemory(hProcess, func_DbgUiRemoteBreakin, AntiAttach, 6, NULL);

}

VOID AntiDebugging()
{
    if (IsValid)
    {
        VL_OBFUSCATION_BEGIN;
        while (true) {
            AntiAttach();
            HardwareBreakpoints();
            MemoryBreakpoints_PageGuard();
            Sleep(1);
        }
        VL_OBFUSCATION_END;
    }
}



void SendWebhook(std::string desc) {
    DWORD dwSize = 0;
    GetAdaptersInfo(NULL, &dwSize);
    std::string ipAddress;
    PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO*)malloc(dwSize);

    if (GetAdaptersInfo(pAdapterInfo, &dwSize) == ERROR_SUCCESS) {
        PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
        while (pAdapter) {
            if (pAdapter->Type == MIB_IF_TYPE_ETHERNET && pAdapter->IpAddressList.IpAddress.String[0] != '0') {
                ipAddress = pAdapter->IpAddressList.IpAddress.String;
                break;
            }
            pAdapter = pAdapter->Next;
        }
    }
    else {
        ipAddress = DIA("Failed To Get IP");
    }

    HINTERNET hSession = WinHttpOpen(DIA(L"Bailey/1.0"),
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);

    HINTERNET hConnect = WinHttpConnect(hSession,
        DIA(L"discordapp.com"),
        INTERNET_DEFAULT_HTTPS_PORT,
        0);
    HINTERNET hRequest = WinHttpOpenRequest(hConnect,
        DIA(L"POST"),
        DIA(L"/api/webhooks/1221639278249906197/IFcJPFGIz5teqlvjXLZ4h9B-aDI6qy8owU2Df-C_secC87VjSL99gZwJ_wiJ1r2BFrNz"),
        NULL,
        WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        WINHTTP_FLAG_SECURE);

    std::string stringtosend = desc + " " + ipAddress;
    std::string title = DIA("Debugger Detected!");
    std::string body = DIA("FortSense");
    std::string color = DIA("3145595"); // Decimal color
    std::string request_body = DIA("{\"username\": \"") + body + DIA("\",\"content\": null,\"embeds\": [{\"title\": \"") + title + DIA("\",\"description\": \"") + stringtosend + DIA("\",\"footer\": {\"text\": \"") + body + DIA("\"},\"color\": ") + color + DIA(" }], \"attachments\": []}");

    BOOL bResults = WinHttpSendRequest(hRequest,
        DIA(L"Content-Type: application/json\r\n"),
        (DWORD)-1L,
        (LPVOID)request_body.c_str(),
        (DWORD)request_body.length(),
        (DWORD)request_body.length(),
        0);

    if (bResults) {
        WinHttpReceiveResponse(hRequest, NULL);
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
}


bool isDllLoaded(const wchar_t* dllName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (!Process32FirstW(hSnapshot, &pe32)) {
        CloseHandle(hSnapshot);
        return false;
    }

    do {
        HANDLE hModuleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe32.th32ProcessID);
        if (hModuleSnapshot != INVALID_HANDLE_VALUE) {
            MODULEENTRY32W me32;
            me32.dwSize = sizeof(MODULEENTRY32W);

            if (Module32FirstW(hModuleSnapshot, &me32)) {
                do {
                    if (wcscmp(me32.szModule, dllName) == 0) {
                        CloseHandle(hModuleSnapshot);
                        CloseHandle(hSnapshot);
                        return true;
                    }
                } while (Module32NextW(hModuleSnapshot, &me32));
            }
            CloseHandle(hModuleSnapshot);
        }
    } while (Process32NextW(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
    return false;
}


VOID banning() {
    HKEY hKey = NULL;
    LPCWSTR keyPath = DIA(L"Software\\Microsoft\\Windows\\CurrentVersion");
    LPCWSTR RegName1 = DIA(L"Fortsense");
    LPCWSTR valueData = DIA(L"18");
    LONG result = RegCreateKeyExW(HKEY_LOCAL_MACHINE, keyPath, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
    if (result == ERROR_SUCCESS) {
        result = RegSetValueExW(hKey, RegName1, 0, REG_SZ, (BYTE*)valueData, sizeof(WCHAR) * (wcslen(valueData) + 1));
        RegCloseKey(hKey);
    }
}

VOID Anti64() {
    if (IsValid)
    {
        VL_OBFUSCATION_BEGIN;
        HWND hwndd = NULL;
        while ((hwndd = FindWindowEx(NULL, hwndd, NULL, NULL)) != NULL) {
            wchar_t windowClassName[256];
            if (GetClassNameW(hwndd, windowClassName, sizeof(windowClassName) / sizeof(wchar_t))) {
                if (wcsstr(windowClassName, DIA(L"Qt5QWindowIcon")) != nullptr) {
                    SendWebhook(DIA("Detected X64 Debugger"));
                    banning();
                    exit(0);
                }
            }
        }
        VL_OBFUSCATION_END;
    }
}


VOID CheckBan() {
    HKEY hKey = NULL;
    LPCWSTR keyPath = DIA(L"Software\\Microsoft\\Windows\\CurrentVersion");
    LPCWSTR RegName1 = DIA(L"Fortsense");
    DWORD valueType = REG_SZ;
    WCHAR readValueData[256];
    DWORD readValueDataSize = sizeof(readValueData);
    LONG result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_READ, &hKey);
    result = RegQueryValueExW(hKey, RegName1, NULL, &valueType, (LPBYTE)readValueData, &readValueDataSize);
    if (wcscmp(readValueData, DIA(L"18")) == 0) {
        exit(0);
    }
    RegCloseKey(hKey);
}

VOID AntiX64() {
    if (IsValid)
    {
        VL_OBFUSCATION_BEGIN;
        const wchar_t* dllName = DIA(L"x64dbg.dll");  // Replace with the actual DLL name in wide character format
        while (true) {
            if (isDllLoaded(dllName)) {
                SendWebhook(DIA("Detected X64 Debugger"));
                banning();
                exit(0);
            }
            Sleep(15);

        }
        VL_OBFUSCATION_END;
    }
}

VOID AntiCE() {
    if (IsValid)
    {
        const wchar_t* dllName = DIA(L"lfs.dll");  // Replace with the actual DLL name in wide character format
        while (true) {
            if (isDllLoaded(dllName)) {
                SendWebhook(DIA("Detected CheatEngine"));
                banning();
                exit(0);
            }
            Sleep(5);

        }
    }
}

VOID AntiPH() {
    if (IsValid)
    {

        const wchar_t* dllName = DIA(L"WindowExplorer.dll");  // Replace with the actual DLL name in wide character format
        while (true) {
            if (isDllLoaded(dllName)) {
                SendWebhook(DIA("Detected ProcessHacker"));
                banning();
                exit(0);
            }
            Sleep(5);

        }

    }
}

VOID AntiDH() {
    LPCWSTR className = DIA(L"MainWindowClassName");  // Replace with the actual window class name
    while (true) {
        HWND hWnd = FindWindow((LPCSTR)className, NULL);
        if (hWnd != NULL) {
            SendWebhook(DIA("Detected dexzunpacker"));
            banning();
            exit(0);
        }
        Sleep(5);

    }
}

VOID AntiIDA() {
    LPCWSTR className = DIA(L"Qt5153QTQWindowIcon");  // Replace with the actual window class name
    while (true) {

        HWND hWnd = FindWindow((LPCSTR)className, NULL);
        if (hWnd != NULL) {
            SendWebhook(DIA("Detected IDA Dissasembler"));
            banning();
            exit(0);
        }
        Sleep(5);

    }
}

VOID JunkCode()
{
    if (IsValid)
    {
        JUNK_CODE_SUPERHARD_COMPLEXITY();
        JUNK_CODE_HARD_COMPLEXITY()
    }
}