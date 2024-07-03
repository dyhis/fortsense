#include <d3d9.h>
#include <dwmapi.h>
#include "SKD.h"
#include "protect\xorstr.h"
#include "protect/lazy.h"
#include "protect/skCrypt.h"
#include "stuff.h"
#include <TlHelp32.h>
#include "imports.h"
namespace DirectX9Interface
{
    IDirect3D9Ex* p_object = NULL;
    IDirect3DDevice9Ex* p_device = NULL;
    D3DPRESENT_PARAMETERS p_params = { NULL };
    RECT game_rect = { NULL };
    MSG message = { NULL };
    HWND hijacked_hwnd = NULL;
    HWND game_wnd = NULL;
}

bool amd = false;
bool nvidia = false;

namespace hijack
{



    __forceinline void hijack_init()
    {
        if (LI_FN(FindWindowA)(skCrypt("CEF-OSC-WIDGET"), skCrypt("NVIDIA GeForce Overlay")))
        {
            nvidia = true;
            DirectX9Interface::hijacked_hwnd = LI_FN(FindWindowA)(skCrypt("CEF-OSC-WIDGET"), skCrypt("NVIDIA GeForce Overlay"));
            NOTIFYICONDATA nid = {};
            nid.cbSize = sizeof(NOTIFYICONDATA);
            nid.hWnd = NULL;
            nid.uID = 1;
            nid.uFlags = NIF_INFO;
            nid.dwInfoFlags = NIIF_INFO;
            strcpy_s(nid.szInfoTitle, ("Fortsense"));
            strcpy_s(nid.szInfo, ("Found NVIDIA GeFore Experience Overlay"));
            Shell_NotifyIcon(NIM_ADD, &nid);
            Sleep(5000);
            Shell_NotifyIcon(NIM_DELETE, &nid);
        }
        else if (LI_FN(FindWindowA)(skCrypt("AMDDVROVERLAYWINDOWCLASS"), skCrypt("amd dvr overlay")))
        {
            amd = true;
            DirectX9Interface::hijacked_hwnd = LI_FN(FindWindowA)(skCrypt("AMDDVROVERLAYWINDOWCLASS"), skCrypt("amd dvr overlay"));
            NOTIFYICONDATA nid = {};
            nid.cbSize = sizeof(NOTIFYICONDATA);
            nid.hWnd = NULL;
            nid.uID = 1;
            nid.uFlags = NIF_INFO;
            nid.dwInfoFlags = NIIF_INFO;
            strcpy_s(nid.szInfoTitle, ("Fortsense"));
            strcpy_s(nid.szInfo, ("Found Radeon AMD Overlay"));
            Shell_NotifyIcon(NIM_ADD, &nid);
            Sleep(5000);
            Shell_NotifyIcon(NIM_DELETE, &nid);
        }
        else
        {
            NOTIFYICONDATA nid = {};
            nid.cbSize = sizeof(NOTIFYICONDATA);
            nid.hWnd = NULL;
            nid.uID = 1;
            nid.uFlags = NIF_INFO;
            nid.dwInfoFlags = NIIF_INFO;
            strcpy_s(nid.szInfoTitle, ("Fortsense"));
            strcpy_s(nid.szInfo, ("NVIDIA/AMD not found!"));
            Shell_NotifyIcon(NIM_ADD, &nid);
            Sleep(8000);
            Shell_NotifyIcon(NIM_DELETE, &nid);
            exit(1);
        }

        LI_FN(ShowWindow)(DirectX9Interface::hijacked_hwnd, SW_SHOW);

        LI_FN(SetWindowLongA)(DirectX9Interface::hijacked_hwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
        LI_FN(SetWindowLongA)(
            DirectX9Interface::hijacked_hwnd,
            -20,
            static_cast<LONG_PTR>(
                static_cast<int>(LI_FN(GetWindowLongA)(DirectX9Interface::hijacked_hwnd, -20)) | 0x20
                )
            );

        if (amd) LI_FN(MoveWindow)(DirectX9Interface::hijacked_hwnd, 0, 0, screen_width, screen_height, false); // only when amd

        //transparency
        MARGINS margin = { -1, -1, -1, -1 };
        LI_FN(DwmExtendFrameIntoClientArea)(
            DirectX9Interface::hijacked_hwnd,
            &margin
            );

        LI_FN(SetLayeredWindowAttributes)(
            DirectX9Interface::hijacked_hwnd,
            NULL,
            0xFF,
            0x02
            );

        // top most
        LI_FN(SetWindowPos)(
            DirectX9Interface::hijacked_hwnd,
            HWND_TOPMOST,
            0, 0, 0, 0,
            0x0002 | 0x0001
            );


        LI_FN(UpdateWindow)(DirectX9Interface::hijacked_hwnd);

    }

}
DWORD assid = 0;

inline uintptr_t get_module_base(uint32_t process_id, LPCTSTR module_name)
{


    uintptr_t base_address = 0;
    const HANDLE snapshot_handle = LI_FN(CreateToolhelp32Snapshot)(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);
    if (snapshot_handle != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 module_entry;
        module_entry.dwSize = sizeof(module_entry);
        if (LI_FN(Module32First)(snapshot_handle, &module_entry))
        {
            do
            {
                if (!lstrcmpi(module_entry.szModule, module_name))
                {
                    base_address = reinterpret_cast<uintptr_t>(module_entry.modBaseAddr);
                    break;
                }
            } while (LI_FN(Module32Next)(snapshot_handle, &module_entry));
        }
    }
    LI_FN(CloseHandle)(snapshot_handle);
    return base_address;
}
#define FortPTR reinterpret_cast< uint64_t >
inline uintptr_t get_module_export(HANDLE process_handle, uintptr_t module_base, const char* export_name)
{


    SIZE_T dummy_read_size;
    IMAGE_DOS_HEADER dos_header = { 0 };
    IMAGE_NT_HEADERS64 nt_headers = { 0 };

    if (!LI_FN(ReadProcessMemory)(process_handle, reinterpret_cast<void*>(module_base), &dos_header, sizeof(dos_header), &dummy_read_size) || dos_header.e_magic != IMAGE_DOS_SIGNATURE ||
        !LI_FN(ReadProcessMemory)(process_handle, reinterpret_cast<void*>(module_base + dos_header.e_lfanew), &nt_headers, sizeof(nt_headers), &dummy_read_size) || nt_headers.Signature != IMAGE_NT_SIGNATURE) {
        return 0;
    }

    const auto export_base = nt_headers.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    const auto export_base_size = nt_headers.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;

    if (!export_base || !export_base_size) {
        return 0;
    }

    const auto export_data = static_cast<PIMAGE_EXPORT_DIRECTORY>(LI_FN(VirtualAlloc)(nullptr, export_base_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
    if (!export_data) {
        return 0;
    }

    if (!LI_FN(ReadProcessMemory)(process_handle, reinterpret_cast<void*>(module_base + export_base), export_data, export_base_size, &dummy_read_size)) {
        LI_FN(VirtualFree)(export_data, 0, MEM_RELEASE);
        return 0;
    }

    const auto delta = FortPTR(export_data) - export_base;

    const auto name_table = reinterpret_cast<uint32_t*>(export_data->AddressOfNames + delta);
    const auto ordinal_table = reinterpret_cast<uint16_t*>(export_data->AddressOfNameOrdinals + delta);
    const auto function_table = reinterpret_cast<uint32_t*>(export_data->AddressOfFunctions + delta);

    for (auto i = 0u; i < export_data->NumberOfNames; ++i)
    {
        const std::string current_function_name = std::string(reinterpret_cast<char*>(name_table[i] + delta));

        if (!_stricmp(current_function_name.c_str(), export_name))
        {
            const auto function_ordinal = ordinal_table[i];
            if (function_table[function_ordinal] <= 0x1000)
                return 0;

            const auto function_address = module_base + function_table[function_ordinal];

            if (function_address >= module_base + export_base && function_address <= module_base + export_base + export_base_size)
            {
                LI_FN(VirtualFree)(export_data, 0, MEM_RELEASE);
                return 0;
            }

            LI_FN(VirtualFree)(export_data, 0, MEM_RELEASE);
            return function_address;
        }
    }

    LI_FN(VirtualFree)(export_data, 0, MEM_RELEASE);
    return 0;
}

inline bool hide_window(uint32_t process_id, HWND window_id, bool hide)
{


    const HANDLE process_handle = LI_FN(OpenProcess)(PROCESS_ALL_ACCESS, false, process_id);
    if (!process_handle || process_handle == INVALID_HANDLE_VALUE)
        return false;

    const uintptr_t user32_base = get_module_base(process_id, SK("user32.dll"));
    if (!user32_base)
        return false;

    const uintptr_t function_pointer = get_module_export(process_handle, user32_base, SK("SetWindowDisplayAffinity"));
    if (!function_pointer)
        return false;

    unsigned char shellcode_buffer[] = "\x48\x89\x4C\x24\x08\x48\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24"
        "\x20\x48\x83\xEC\x38\x48\xB9\xED\xFE\xAD\xDE\xED\xFE\x00\x00\x48\xC7\xC2\xAD"
        "\xDE\x00\x00\x48\xB8\xAD\xDE\xED\xFE\xAD\xDE\x00\x00\xFF\xD0\x48\x83\xC4\x38"
        "\x48\x8B\x4C\x24\x08\x48\x8B\x54\x24\x10\x4C\x8B\x44\x24\x18\x4C\x8B\x4C\x24"
        "\x20\xC3";

    const uint32_t mask = hide ? 0x00000011 : 0x00000000;

    *reinterpret_cast<uintptr_t*>(shellcode_buffer + 26) = reinterpret_cast<uintptr_t>(window_id);
    *reinterpret_cast<uint32_t*>(shellcode_buffer + 37) = mask;
    *reinterpret_cast<uintptr_t*>(shellcode_buffer + 43) = function_pointer;

    void* allocated_base = VirtualAllocEx(process_handle, 0x0, sizeof(shellcode_buffer), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (!allocated_base)
        return false;

    SIZE_T dummy_size;
    if (!LI_FN(WriteProcessMemory)(process_handle, allocated_base, shellcode_buffer, sizeof(shellcode_buffer), &dummy_size))
        return false;

    HANDLE thread_handle = LI_FN(CreateRemoteThread)(process_handle, nullptr, 0, static_cast<LPTHREAD_START_ROUTINE>(allocated_base), nullptr, 0, nullptr);
    if (!thread_handle || thread_handle == INVALID_HANDLE_VALUE)
        return false;

    LI_FN(Sleep)(1000);

    LI_FN(memset)(shellcode_buffer, 0, sizeof(shellcode_buffer));
    if (!LI_FN(WriteProcessMemory)(process_handle, allocated_base, shellcode_buffer, sizeof(shellcode_buffer), &dummy_size))
        return false;

    return true;
}


void hide_overlay() {


    HWND window;
    if (nvidia)
    {
        window = FindWindowA_Spoofed(SK("CEF-OSC-WIDGET"), SK("NVIDIA GeForce Overlay"));
    }
    else
    {
        window = FindWindowA_Spoofed(SK("AMDDVROVERLAYWINDOWCLASS"), SK("amd dvr overlay"));

    }

    ITaskbarList* pTaskList = NULL;
    HRESULT initRet = CoInitialize(NULL);
    HRESULT createRet = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList, (LPVOID*)&pTaskList);

    LI_FN(GetWindowThreadProcessId).get()(window, &assid);
    hide_window(assid, window, true);

    if (createRet == S_OK) {
        pTaskList->DeleteTab(window);

        pTaskList->Release();
    }

    CoUninitialize();
}

void show_overlay() {


    HWND window;
    if (nvidia)
    {
        window = FindWindowA_Spoofed(SK("CEF-OSC-WIDGET"), SK("NVIDIA GeForce Overlay"));
    }
    else
    {
        window = FindWindowA_Spoofed(SK("AMDDVROVERLAYWINDOWCLASS"), SK("amd dvr overlay"));

    }
    ITaskbarList* pTaskList = NULL;
    HRESULT initRet = CoInitialize(NULL);
    HRESULT createRet = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList, (LPVOID*)&pTaskList);

    LI_FN(GetWindowThreadProcessId).get()(window, &assid);
   hide_window(assid, window, false);

    if (createRet == S_OK) {
        pTaskList->AddTab(window);

        pTaskList->Release();
    }

    CoUninitialize();
}
