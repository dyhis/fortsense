#include <windows.h>
#include <dwmapi.h>
#include "ex.h"
#include "LazyImporter.h"

inline namespace hotkeys
{
	inline int aimkey = VK_RBUTTON;
}

inline BOOL(*_GetCursorPos)(LPPOINT lpPoint) = nullptr;
inline BOOL(*_SetLayeredWindowAttributes)(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags) = nullptr;
inline HRESULT(*_DwmExtendFrameIntoClientArea)(HWND hwnd, const MARGINS* pMarInset) = nullptr;
inline LRESULT(*_DefWindowProc)(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) = nullptr;
inline HWND(*_CreateWindowExA)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) = nullptr;
inline LONG(*_SetWindowLongA)(HWND hwnd, int nIndex, LONG dwNewLong) = nullptr;
inline LONG(*_GetWindowLongA)(HWND hwnd, int nIndex) = nullptr;
inline ATOM(*_RegisterClassExA)(const WNDCLASSEXA*) = nullptr;
inline int(*_GetSystemMetrics)(int nIndex) = nullptr;
inline HWND(*_FindWindowA)(LPCSTR lpClassName, LPCSTR lpWindowName) = nullptr;
inline SHORT(*_GetAsyncKeyState)(int vKey) = nullptr;
inline HWND(*_GetForegroundWindow)() = nullptr;
inline BOOL(*_MoveWindow)(HWND hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint) = nullptr;
inline BOOL(*_PeekMessageA)(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg) = nullptr;
inline BOOL(*_TranslateMessage)(const MSG* lpMsg) = nullptr;
inline LRESULT(*_DispatchMessageA)(const MSG* lpMsg) = nullptr;
inline HANDLE(*_CreateThread)(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE     lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId) = nullptr;
inline BOOL(*_CloseHandle)(HANDLE hObject) = nullptr;
inline BOOL(*_SetWindowPos)(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags) = nullptr;
inline BOOL(*_ShowWindow)(HWND hWnd, int nCmdShow) = nullptr;
inline BOOL(*_DestroyWindow)(HWND hWnd) = nullptr;
inline BOOL(*_SetForegroundWindow)(HWND hWnd) = nullptr;
inline BOOL(*_SetWindowDisplayAffinity)(HWND hWnd, DWORD dwAffinity) = nullptr;
inline BOOL(*_mouse_event)(DWORD, DWORD, DWORD, DWORD, ULONG_PTR) = nullptr;
inline BOOL(*_UpdateWindow)(HWND) = nullptr;
inline BOOL(*_GetClientRect)(HWND, LPRECT) = nullptr;
inline BOOL(*_ClientToScreen)(HWND, LPPOINT) = nullptr;
inline BOOL(*_DeviceIoControl)(HANDLE, DWORD, LPVOID, DWORD, LPVOID, DWORD, LPDWORD, LPOVERLAPPED) = nullptr;
inline BOOL(*_GetWindowRect)(HWND, LPRECT) = nullptr;
inline HWND(*_GetDesktopWindow)() = nullptr;


inline void mouse_interface()
{
	auto user32STRING = E("user32.dll");
	auto Kernel32STRING = E("Kernel32.dll");
	auto win32uSTRING = E("win32u.dll");
	auto dwmapiSTRING = E("dwmapi.dll");
	auto msvcrtSTRING = E("msvcrt.dll");

	user32STRING.encrypt();
	Kernel32STRING.encrypt();
	win32uSTRING.encrypt();
	dwmapiSTRING.encrypt();
	msvcrtSTRING.encrypt();

	HMODULE user32 = LI_FN(LoadLibraryA).get()(user32STRING.decrypt());
	HMODULE kernel32 = LI_FN(LoadLibraryA).get()(Kernel32STRING.decrypt());
	HMODULE win32u = LI_FN(LoadLibraryA).get()(win32uSTRING.decrypt());
	HMODULE dwmapi = LI_FN(LoadLibraryA).get()(dwmapiSTRING.decrypt());
	HMODULE msvcrt = LI_FN(LoadLibraryA).get()(msvcrtSTRING.decrypt());

	user32STRING.encrypt();
	Kernel32STRING.encrypt();
	win32uSTRING.encrypt();
	dwmapiSTRING.encrypt();
	msvcrtSTRING.encrypt();




	if (!win32u) {
		return;
	}

	if (!user32) {
		return;
	}

	if (!dwmapi) {
		return;
	}

	if (!kernel32) {
		return;
	}

	if (!msvcrt) {
		return;
	}


	void* NtUserInjectMouseInputAddress = (void*)LI_FN(GetProcAddress).get()(win32u, skCrypt("NtUserInjectMouseInput"));
	void* GetCursorPos = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("GetCursorPos"));
	void* SetLayeredWindowAttributes = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("SetLayeredWindowAttributes"));
	void* DwmExtendFrameIntoClientArea = (void*)LI_FN(GetProcAddress).get()(dwmapi, skCrypt("DwmExtendFrameIntoClientArea"));
	void* DefWindowProcA = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("DefWindowProcA"));
	void* CreateWindowExA = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("CreateWindowExA"));
	void* SetWindowLongA = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("SetWindowLongA"));
	void* GetWindowLongA = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("GetWindowLongA"));
	void* RegisterClassExA = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("RegisterClassExA"));
	void* GetSystemMetrics = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("GetSystemMetrics"));
	void* FindWindowA = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("FindWindowA"));
	void* GetAsyncKeyState = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("GetAsyncKeyState"));
	void* GetForegroundWindow = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("GetForegroundWindow"));
	void* MoveWindow = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("MoveWindow"));
	void* PeekMessageA = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("PeekMessageA"));
	void* TranslateMessage = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("TranslateMessage"));
	void* DispatchMessageA = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("DispatchMessageA"));
	void* CreateThread = (void*)LI_FN(GetProcAddress).get()(kernel32, skCrypt("CreateThread"));
	void* DeviceIoControl = (void*)LI_FN(GetProcAddress).get()(kernel32, skCrypt("DeviceIoControl"));
	void* SetWindowPos = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("SetWindowPos"));
	void* CloseHandle = (void*)LI_FN(GetProcAddress).get()(kernel32, skCrypt("CloseHandle"));
	void* ShowWindow = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("ShowWindow"));
	void* Destroywindow = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("DestroyWindow"));
	void* SetForegroundwindow = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("SetForegroundWindow"));
	void* SetWindowDisplayAffinity = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("SetWindowDisplayAffinity"));
	void* mouzeevent = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("mouse_event"));
	void* updetewindow = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("UpdateWindow"));
	void* clientrect = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("GetClientRect"));
	void* clienttoscreen = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("ClientToScreen"));
	void* getwindowrect = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("GetWindowRect"));
	void* getdesktopwindow = (void*)LI_FN(GetProcAddress).get()(user32, skCrypt("GetDesktopWindow"));

	if (!NtUserInjectMouseInputAddress) {
		return;
	}

	if (!GetCursorPos) {
		return;
	}

	if (!SetLayeredWindowAttributes) {
		return;
	}

	if (!DwmExtendFrameIntoClientArea) {
		return;
	}

	if (!SetForegroundwindow) {
		return;
	}

	if (!DefWindowProcA) {
		return;
	}

	if (!CreateWindowExA) {
		return;
	}

	if (!SetWindowLongA) {
		return;
	}

	if (!GetWindowLongA) {
		return;
	}

	if (!RegisterClassExA) {
		return;
	}

	if (!GetSystemMetrics) {
		return;
	}

	if (!FindWindowA) {
		return;
	}

	if (!GetAsyncKeyState) {
		return;
	}

	if (!GetForegroundWindow) {
		return;
	}

	if (!MoveWindow) {
		return;
	}

	if (!PeekMessageA) {
		return;
	}

	if (!TranslateMessage) {
		return;
	}

	if (!DispatchMessageA) {
		return;
	}

	if (!CreateThread) {
		return;
	}

	if (!CloseHandle) {
		return;
	}

	if (!SetWindowPos) {
		return;
	}

	if (!ShowWindow) {
		return;
	}

	if (!SetWindowDisplayAffinity) {
		return;
	}

	if (!mouzeevent) {
		return;
	}

	if (!updetewindow) {
		return;
	}

	if (!clientrect) {
		return;
	}

	if (!clienttoscreen) {
		return;
	}

	if (!DeviceIoControl) {
		return;
	}

	if (!Destroywindow) {
		return;
	}

	if (!getwindowrect) {
		return;
	}

	if (!getdesktopwindow) {
		return;
	}


	*(void**)&_GetCursorPos = GetCursorPos;
	*(void**)&_SetLayeredWindowAttributes = SetLayeredWindowAttributes;
	*(void**)&_DwmExtendFrameIntoClientArea = DwmExtendFrameIntoClientArea;
	*(void**)&_DefWindowProc = DefWindowProcA;
	*(void**)&_CreateWindowExA = CreateWindowExA;
	*(void**)&_SetWindowLongA = SetWindowLongA;
	*(void**)&_GetWindowLongA = GetWindowLongA;
	*(void**)&_RegisterClassExA = RegisterClassExA;
	*(void**)&_GetSystemMetrics = GetSystemMetrics;
	*(void**)&_FindWindowA = FindWindowA;
	*(void**)&_GetAsyncKeyState = GetAsyncKeyState;
	*(void**)&_GetForegroundWindow = GetForegroundWindow;
	*(void**)&_MoveWindow = MoveWindow;
	*(void**)&_PeekMessageA = PeekMessageA;
	*(void**)&_TranslateMessage = TranslateMessage;
	*(void**)&_DispatchMessageA = DispatchMessageA;
	*(void**)&_CreateThread = CreateThread;
	*(void**)&_CloseHandle = CloseHandle;
	*(void**)&_SetWindowPos = SetWindowPos;
	*(void**)&_ShowWindow = ShowWindow;
	*(void**)&_SetWindowDisplayAffinity = SetWindowDisplayAffinity;
	*(void**)&_mouse_event = mouzeevent;
	*(void**)&_UpdateWindow = updetewindow;
	*(void**)&_GetClientRect = clientrect;
	*(void**)&_ClientToScreen = clienttoscreen;
	*(void**)&_DeviceIoControl = DeviceIoControl;
	*(void**)&_DestroyWindow = Destroywindow;
	*(void**)&_SetForegroundWindow = SetForegroundwindow;
	*(void**)&_GetWindowRect = getwindowrect;
	*(void**)&_GetDesktopWindow = getdesktopwindow;

	LI_FN(FreeLibrary).get()(kernel32);
	LI_FN(FreeLibrary).get()(user32);
	LI_FN(FreeLibrary).get()(win32u);
	LI_FN(FreeLibrary).get()(dwmapi);
	LI_FN(FreeLibrary).get()(msvcrt);


}

inline BOOL GetCursorPosA_Spoofed(LPPOINT lpPoint)
{
	

	return _GetCursorPos(lpPoint);
}

inline BOOL SetLayeredWindowAttributes_Spoofed(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
{
	

	return _SetLayeredWindowAttributes(hwnd, crKey, bAlpha, dwFlags);
}

inline HRESULT __stdcall DwmExtendFrameIntoClientArea_Spoofed(HWND hwnd, const MARGINS* pMarInset)
{
	

	return _DwmExtendFrameIntoClientArea(hwnd, pMarInset);
}

inline LRESULT __stdcall DefWindowProcA_Spoofed(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	

	return _DefWindowProc(hwnd, Msg, wParam, lParam);
}

inline HWND __stdcall CreateWindowExA_Spoofed(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	

	return _CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

inline LONG __stdcall SetWindowLongA_Spoofed(HWND hwnd, int nIndex, LONG dwNewLong)
{
	

	return _SetWindowLongA(hwnd, nIndex, dwNewLong);
}

inline LONG __stdcall GetWindowLongA_Spoofed(HWND hwnd, int nIndex)
{
	

	return _GetWindowLongA(hwnd, nIndex);
}

inline ATOM __stdcall RegisterClassExA_Spoofed(const WNDCLASSEXA* softaim) {
	

	return _RegisterClassExA(softaim);
}

inline int __stdcall GetSystemMetrics_Spoofed(int nIndex) {
	

	return _GetSystemMetrics(nIndex);
}

inline HWND __stdcall FindWindowA_Spoofed(LPCSTR lpClassName, LPCSTR lpWindowName) {
	

	return _FindWindowA(lpClassName, lpWindowName);
}

inline HWND __stdcall GetDesktopWindow_Spoofed() {
	

	return _GetDesktopWindow();
}

inline HWND __stdcall GetForegroundWindow_Spoofed() {
	

	return _GetForegroundWindow();
}

inline SHORT GetAsyncKeyState_Spoofed(int vKey) {
	

	return _GetAsyncKeyState(vKey);
}

inline SHORT MoveWindow_Spoofed(HWND hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint) {
	

	return _MoveWindow(hWnd, X, Y, nWidth, nHeight, bRepaint);
}

inline BOOL __stdcall PeekMessageA_Spoofed(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg) {
	

	return _PeekMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
}

inline BOOL __stdcall TranslateMessage_Spoofed(const MSG* lpMsg) {
	

	return _TranslateMessage(lpMsg);
}

inline LRESULT __stdcall DispatchMessageA_Spoofed(const MSG* lpMsg) {
	

	return _DispatchMessageA(lpMsg);
}

inline HANDLE __stdcall CreateThread_Spoofed(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId) {
	

	return _CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
}

inline BOOL __stdcall CloseHandle_Spoofed(HANDLE hObject) {
	

	return _CloseHandle(hObject);
}

inline BOOL __stdcall SetWindowPos_Spoofed(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags) {
	

	return _SetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
}

inline BOOL __stdcall ShowWindow_Spoofed(HWND hWnd, int nCmdShow) {
	

	return _ShowWindow(hWnd, nCmdShow);
}

inline BOOL __stdcall SetWindowDisplayAffinity_Spoofed(HWND hWnd, DWORD dwAffinity) {
	

	return _SetWindowDisplayAffinity(hWnd, dwAffinity);
}

inline BOOL __stdcall mouse_event_Spoofed(DWORD dwFlags, DWORD dx, DWORD dy, DWORD dwData, ULONG_PTR dwExtraInfo) {
	

	return _mouse_event(dwFlags, dx, dy, dwData, dwExtraInfo);
}

inline BOOL __stdcall UpdateWindow_Spoofed(HWND hWnd) {
	

	return _UpdateWindow(hWnd);
}

inline BOOL __stdcall GetClientRect_Spoofed(HWND hWnd, LPRECT lpRect) {
	

	return _GetClientRect(hWnd, lpRect);
}

inline BOOL __stdcall mem::DeviceIoControl_Spoofed(HANDLE hDevice, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped) {
	

	return _DeviceIoControl(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesReturned, lpOverlapped);
}

inline BOOL __stdcall mem::CloseHandleA_Spoofed(HANDLE hObject) {
	

	return _CloseHandle(hObject);
}

inline BOOL __stdcall DestroyWindow_Spoofed(HWND hWnd)
{
	

	return _DestroyWindow(hWnd);
}

inline BOOL __stdcall SetForegroundWindow_Spoofed(HWND hWnd)
{
	

	return _SetForegroundWindow(hWnd);
}

inline BOOL __stdcall GetWindowRect_Spoofed(HWND hWnd, LPRECT lpRect)
{
	

	return _GetWindowRect(hWnd, lpRect);
}