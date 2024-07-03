#pragma once

#include <Windows.h>
#include <dwmapi.h>
#include "protect/lazy.h"
#include "protect/skCrypt.h"

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
inline BOOL(*_UpdateWindow)(HWND) = nullptr;

inline void hide_LI_FNs()
{
	auto user32 = LI_FN(LoadLibraryA).safe_cached()(SK("user32.dll"));
	auto kernel32 = LI_FN(LoadLibraryA).safe_cached()(SK("Kernel32.dll"));
	auto dwmapi = LI_FN(LoadLibraryA).safe_cached()(SK("Dwmapi.dll"));

	if (!user32)
	{
		return;
	}

	if (!dwmapi)
	{
		return;
	}

	if (!kernel32)
	{
		return;
	}

	void* GetCursorPos = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("GetCursorPos"));
	void* SetLayeredWindowAttributes = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("SetLayeredWindowAttributes"));
	void* DwmExtendFrameIntoClientArea = (void*)LI_FN(GetProcAddress).safe_cached()(dwmapi, SK("DwmExtendFrameIntoClientArea"));
	void* DefWindowProcA = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("DefWindowProcA"));
	void* CreateWindowExA = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("CreateWindowExA"));
	void* SetWindowLongA = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("SetWindowLongA"));
	void* GetWindowLongA = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("GetWindowLongA"));
	void* RegisterClassExA = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("RegisterClassExA"));
	void* GetSystemMetrics = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("GetSystemMetrics"));
	void* FindWindowA = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("FindWindowA"));
	void* GetAsyncKeyState = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("GetAsyncKeyState"));
	void* GetForegroundWindow = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("GetForegroundWindow"));
	void* MoveWindow = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("MoveWindow"));
	void* PeekMessageA = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("PeekMessageA"));
	void* TranslateMessage = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("TranslateMessage"));
	void* DispatchMessageA = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("DispatchMessageA"));
	void* CreateThread = (void*)LI_FN(GetProcAddress).safe_cached()(kernel32, SK("CreateThread"));
	void* SetWindowPos = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("SetWindowPos"));
	void* CloseHandle = (void*)LI_FN(GetProcAddress).safe_cached()(kernel32, SK("CloseHandle"));
	void* ShowWindow = (void*)LI_FN(GetProcAddress).safe_cached()(user32, SK("ShowWindow"));
	void* updetewindow = (void*)LI_FN(GetProcAddress).get()(user32, SK("UpdateWindow"));

	if (!GetCursorPos)
	{
		return;
	}

	if (!SetLayeredWindowAttributes)
	{
		return;
	}

	if (!DwmExtendFrameIntoClientArea)
	{
		return;
	}

	if (!DefWindowProcA)
	{
		return;
	}

	if (!CreateWindowExA)
	{
		return;
	}

	if (!SetWindowLongA)
	{
		return;
	}

	if (!GetWindowLongA)
	{
		return;
	}

	if (!RegisterClassExA)
	{
		return;
	}

	if (!GetSystemMetrics)
	{
		return;
	}

	if (!FindWindowA)
	{
		return;
	}

	if (!GetAsyncKeyState)
	{
		return;
	}

	if (!GetForegroundWindow)
	{
		return;
	}

	if (!MoveWindow)
	{
		return;
	}

	if (!PeekMessageA)
	{
		return;
	}

	if (!TranslateMessage)
	{
		return;
	}

	if (!DispatchMessageA)
	{
		return;
	}

	if (!CreateThread)
	{
		return;
	}

	if (!CloseHandle)
	{
		return;
	}

	if (!SetWindowPos)
	{
		return;
	}

	if (!ShowWindow)
	{
		return;
	}

	if (!updetewindow)
	{
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
	*(void**)&_UpdateWindow = updetewindow;

	LI_FN(FreeLibrary).get()(kernel32);
	LI_FN(FreeLibrary).get()(user32);
	LI_FN(FreeLibrary).get()(dwmapi);
}

inline BOOL GetCursorPosA(LPPOINT lpPoint)
{
	SPOOF_FUNC

	return _GetCursorPos(lpPoint);
}

inline BOOL SetLayeredWindowAttributes_Spoofed(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
{
	SPOOF_FUNC

	return _SetLayeredWindowAttributes(hwnd, crKey, bAlpha, dwFlags);
}

inline HRESULT __stdcall DwmExtendFrameIntoClientArea_Spoofed(HWND hwnd, const MARGINS* pMarInset)
{
	SPOOF_FUNC

	return _DwmExtendFrameIntoClientArea(hwnd, pMarInset);
}

inline LRESULT __stdcall DefWindowProcA_Spoofed(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	SPOOF_FUNC

	return _DefWindowProc(hwnd, Msg, wParam, lParam);
}

inline HWND __stdcall CreateWindowExA_Spoofed(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	SPOOF_FUNC

	return _CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

inline LONG __stdcall SetWindowLongA_Spoofed(HWND hwnd, int nIndex, LONG dwNewLong)
{
	SPOOF_FUNC

	return _SetWindowLongA(hwnd, nIndex, dwNewLong);
}

inline LONG __stdcall GetWindowLongA_Spoofed(HWND hwnd, int nIndex)
{

	SPOOF_FUNC
	return _GetWindowLongA(hwnd, nIndex);
}

inline ATOM __stdcall RegisterClassExA_Spoofed(const WNDCLASSEXA* classame)
{
	SPOOF_FUNC

	return _RegisterClassExA(classame);
}

inline int __stdcall GetSystemMetrics_Spoofed(int nIndex)
{

	SPOOF_FUNC
	return _GetSystemMetrics(nIndex);
}

inline HWND __stdcall FindWindowA_Spoofed(LPCSTR lpClassName, LPCSTR lpWindowName)
{
	SPOOF_FUNC

	return _FindWindowA(lpClassName, lpWindowName);
}

inline BOOL __stdcall UpdateWindow_Spoofed(HWND hWnd) {

	SPOOF_FUNC
	return _UpdateWindow(hWnd);
}

inline HWND __stdcall GetForegroundWindow_Spoofed()
{
	SPOOF_FUNC

	return _GetForegroundWindow();
}

inline SHORT GetAsyncKeyState_Spoofed(int vKey)
{
	SPOOF_FUNC

	return _GetAsyncKeyState(vKey);
}

inline SHORT MoveWindow_Spoofed(HWND hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint)
{
	SPOOF_FUNC

	return _MoveWindow(hWnd, X, Y, nWidth, nHeight, bRepaint);
}

inline BOOL __stdcall PeekMessageA_Spoofed(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{

	SPOOF_FUNC
	return _PeekMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
}

inline BOOL __stdcall TranslateMessage_Spoofed(const MSG* lpMsg)
{
	SPOOF_FUNC

	return _TranslateMessage(lpMsg);
}

inline LRESULT __stdcall DispatchMessageA_Spoofed(const MSG* lpMsg)
{
	SPOOF_FUNC

	return _DispatchMessageA(lpMsg);
}

inline HANDLE __stdcall CreateThread_Spoofed(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId)
{
	SPOOF_FUNC

	return _CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
}

inline BOOL __stdcall CloseHandle_Spoofed(HANDLE hObject)
{
	SPOOF_FUNC

	return _CloseHandle(hObject);
}

inline BOOL __stdcall SetWindowPos_Spoofed(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
{
	SPOOF_FUNC

	return _SetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
}

inline BOOL __stdcall ShowWindow_Spoofed(HWND hWnd, int nCmdShow)
{
	SPOOF_FUNC

	return _ShowWindow(hWnd, nCmdShow);
}


enum TYPES
{
	move = 1,
	left_up = 4,
	left_down = 2,
	right_up = 8,
	right_down = 16
};

struct MOUSE_INFO
{
	int move_direction_x;
	int move_direction_y;
	unsigned int mouse_data;
	TYPES mouse_options;
	unsigned int time_offset_in_miliseconds;
	void* extra_info;
};

inline bool(*MOVE)(MOUSE_INFO*, int) = nullptr;

enum INFO : int {
	FAIL = 0,
	SUCCESS = 1
};

namespace MOUSE_HOOK
{
	class c_hooking {
	public:
		auto WINAPI HOOK() -> BOOLEAN {

			auto win32u = LI_FN(LoadLibraryA).safe_cached()(("win32u.dll"));
			void* ADDRESS = (void*)LI_FN(GetProcAddress).safe_cached()(win32u, ("NtUserInjectMouseInput"));
			if (!ADDRESS) return INFO::FAIL;
			*(void**)&MOVE = ADDRESS;
			LI_FN(FreeLibrary).get()(win32u);
			return INFO::SUCCESS;
		}
	};
} static inline MOUSE_HOOK::c_hooking* hook_mouse = new MOUSE_HOOK::c_hooking();

namespace MOUSE_MOVEMENT {
	class c_movement {
	public:
		auto WINAPI Move(int X, int Y, unsigned int time = 0) -> BOOLEAN {

			MOUSE_INFO info = {};
			info.mouse_data = 0;
			info.mouse_options = TYPES::move;
			info.move_direction_x = X;
			info.move_direction_y = Y;
			info.time_offset_in_miliseconds = time;
			return MOVE(&info, 1);
		}

		bool Left(int x = 0, int y = 0)
		{

			MOUSE_INFO info = {};
			info.mouse_data = 0;
			info.mouse_options = TYPES::left_down;
			info.move_direction_x = x;
			info.move_direction_y = y;
			info.time_offset_in_miliseconds = 0;
			return MOVE(&info, 1);
		}

		bool Up(int x = 0, int y = 0)
		{

			MOUSE_INFO info = {};
			info.mouse_data = 0;
			info.mouse_options = TYPES::left_up;
			info.move_direction_x = x;
			info.move_direction_y = y;
			info.time_offset_in_miliseconds = 0;
			return MOVE(&info, 1);
		}
	};
} static MOUSE_MOVEMENT::c_movement* MOVE_MOUSE = new MOUSE_MOVEMENT::c_movement();

float RandomFloat(float min, float max) {
	// Initialize random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);

	// Generate and return random float
	return dis(gen);
}

auto mouse(float x, float y) -> void {
			vector3 center(screen_width / 2,screen_height / 2, 0);
			vector3 target(0, 0, 0);

			if (x != 0) {
				if (x > center.x) {
					target.x = -(center.x - x);
					target.x /= (smoothingx);
					if (target.x + center.x > center.x * 2)
						target.x = 0;
				}

				if (x < center.x) {
					target.x = x - center.x;
					target.x /= (smoothingx);
					if (target.x + center.x < 0)
						target.x = 0;
				}
			}
			if (y != 0) {
				if (y > center.y) {
					target.y = -(center.y - y);
					target.y /= (smoothingy);
					if (target.y + center.y > center.y * 2)
						target.y = 0;
				}

				if (y < center.y) {
					target.y = y - center.y;
					target.y /= (smoothingy);
					if (target.y + center.y < 0)
						target.y = 0;
				}
			}

			const float snapThreshold = 1.0f;
			if (std::abs(target.x) < snapThreshold) {
				target.x = 0;
			}
			if (std::abs(target.y) < snapThreshold) {
				target.y = 0;
			}

			if (humanization)
			{
				float targetx_min = target.x - 1;
				float targetx_max = target.x + 1;

				float targety_min = target.y - 1;
				float targety_max = target.y + 1;

				float offset_x = RandomFloat(targetx_min, targetx_max);
				float offset_y = RandomFloat(targety_min, targety_max);

				MOVE_MOUSE->Move(static_cast<int>((float)offset_x), static_cast<int>((float)offset_y / 1));
			}
			else
			{
				MOVE_MOUSE->Move(target.x, target.y);
			}



}

auto mouse1(float x, float y) -> void {
	vector3 center(screen_width / 2, screen_height / 2, 0);
	vector3 target(0, 0, 0);

	if (x != 0) {
		if (x > center.x) {
			target.x = -(center.x - x);
			target.x /= (smoothingx);
			if (target.x + center.x > center.x * 2)
				target.x = 0;
		}

		if (x < center.x) {
			target.x = x - center.x;
			target.x /= (smoothingx);
			if (target.x + center.x < 0)
				target.x = 0;
		}
	}
	if (y != 0) {
		if (y > center.y) {
			target.y = -(center.y - y);
			target.y /= (smoothingy);
			if (target.y + center.y > center.y * 2)
				target.y = 0;
		}

		if (y < center.y) {
			target.y = y - center.y;
			target.y /= (smoothingy);
			if (target.y + center.y < 0)
				target.y = 0;
		}
	}

	const float snapThreshold = 1.0f;
	if (std::abs(target.x) < snapThreshold) {
		target.x = 0;
	}
	if (std::abs(target.y) < snapThreshold) {
		target.y = 0;
	}

	if (humanization)
	{
		float targetx_min = target.x - 1;
		float targetx_max = target.x + 1;

		float targety_min = target.y - 1;
		float targety_max = target.y + 1;

		float offset_x = RandomFloat(targetx_min, targetx_max);
		float offset_y = RandomFloat(targety_min, targety_max);

		mouse_event(MOUSEEVENTF_MOVE, static_cast<int>((float)offset_x), static_cast<int>((float)offset_y / 1), NULL, NULL);
	}
	else
	{
		mouse_event(MOUSEEVENTF_MOVE, target.x, target.y, NULL, NULL);
	}


}
