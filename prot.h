
#include <windows.h>
#include <psapi.h>
#include <time.h>
#include <stdio.h>
#include <thread>
#include <vector>
inline bool IsValid = true;

#define RG_MAGIC_0 0x20170408
#define RG_MAGIC_1 0x12345678
#define RG_MAGIC_2 0x87654321
#define RG_DATA_SIZE 0x10000
#define MODULE_FIRST 0
#define MODULE_EXE 0
#define MODULE_NTDLL 1
#define MODULE_KERNEL32 2
#define MODULE_KERNELBASE 3
#define MODULE_LAST 3
#define PADDING(p, size) ((SIZE_T)((SIZE_T)(p) / (SIZE_T)(size) * (SIZE_T)(size) + ((SIZE_T)(p) % (SIZE_T)(size) ? (SIZE_T)(size) : 0)))
#define GetPtr(base, offset) ((PVOID)((SIZE_T)(base) + (SIZE_T)(offset)))
#define GetOffset(src, dst) ((SIZE_T)((SIZE_T)(dst) - (SIZE_T)(src)))
#define GetNtHeader(base) ((PIMAGE_NT_HEADERS)((SIZE_T)(base) + (SIZE_T)((PIMAGE_DOS_HEADER)(base))->e_lfanew))
#define TO_STRING(param) #param
#define APICALL(api_name) ((decltype(&api_name))RG_GetApi(DIA(TO_STRING(api_name))))
#define APICALL_FROM_MODULE(index, api_name) ((decltype(&api_name))RG_GetApi(DIA(TO_STRING(api_name)), index))
#define IS_ENABLED(OPTION) (OPTION & RG_ENABLE)
#define PAGE_SIZE 0x1000
#define ALLOCATION_GRANULARITY 0x10000
#define CURRENT_PROCESS ((HANDLE)-1)
#define CURRENT_THREAD ((HANDLE)-2)
#ifdef _WIN64
#define MEMORY_END 0x7FFFFFFF0000
#else
#define MEMORY_END 0x7FFF0000
#endif

typedef struct _baileyED_MODULE_INFO
{
    PVOID module_base;
    HANDLE section;
} baileyED_MODULE_INFO, * PbaileyED_MODULE_INFO;

typedef struct _RG_DATA
{
    SIZE_T magic[3];
    baileyED_MODULE_INFO rmi[RG_DATA_SIZE - sizeof(SIZE_T) * 3];
} RG_DATA, * PRG_DATA;

extern PRG_DATA rgdata;


enum RG_REPORT_CODE
{
    REPORT_UNKNOWN,
    REPORT_THREAD_START_ADDRESS,
    REPORT_THREAD_PROTECTION,
    REPORT_DLL_INJECTION_KERNEL32_LoadLibraryA,
    REPORT_DLL_INJECTION_KERNEL32_LoadLibraryW,
    REPORT_DLL_INJECTION_KERNEL32_LoadLibraryExA,
    REPORT_DLL_INJECTION_KERNEL32_LoadLibraryExW,
    REPORT_DLL_INJECTION_KERNELBASE_LoadLibraryA,
    REPORT_DLL_INJECTION_KERNELBASE_LoadLibraryW,
    REPORT_DLL_INJECTION_KERNELBASE_LoadLibraryExA,
    REPORT_DLL_INJECTION_KERNELBASE_LoadLibraryExW,
    REPORT_DLL_INJECTION_NTDLL_LdrLoadDll,
    REPORT_MEMORY_SUSPICIOUS,
    REPORT_MEMORY_UNLOCKED,
    REPORT_INTEGRITY_SECTION_CHECK,
    REPORT_INTEGRITY_CRC64_CHECK,
    REPORT_INVALID_APICALL,
    REPORT_DEBUG_HW_BREAKPOINT_0,
    REPORT_DEBUG_HW_BREAKPOINT_1,
    REPORT_DEBUG_HW_BREAKPOINT_2,
    REPORT_DEBUG_HW_BREAKPOINT_3,
    REPORT_DEBUG_SW_BREAKPOINT,
    REPORT_DEBUG_SINGLE_STEP,
    REPORT_DEBUG_PAGE_GUARD,
};

enum PE_TYPE
{
    PE_TYPE_FILE,
    PE_TYPE_IMAGE
};

enum THREAD_CHECK
{
    TC_TlsCallback,
    TC_ThreadCallback,
    TC_DllCallback
};

enum PTR_CHECK
{
    PC_EXECUTABLE,
    PC_IMAGE_SIZE
};

#include <iostream>
#include <Windows.h>



#define JUNK_CODE_FUNCTION_1() { \
    int counter = 5; \
    while (counter < 30) { \
        counter += 5; \
        if (counter % 3 == 0) { \
            counter *= 2; \
        } else { \
            counter -= 3; \
        } \
    } \
    \
    if (counter > 15) { \
        for (int i = 0; i < 6; ++i) { \
            counter += i * 7; \
            if (counter % 4 == 0) { \
                counter -= i; \
            } else { \
                counter += 3 * i; \
            } \
        } \
    } else { \
        counter -= 8; \
        if (counter < 10) { \
            counter += 5; \
        } else { \
            counter -= 2; \
        } \
    } \
    \
    do { \
        counter += 4; \
        if (counter % 6 == 0) { \
            counter -= 5; \
        } else { \
            counter += 6; \
        } \
    } while (counter % 5 != 0); \
    \
    while (counter > 0) { \
        counter -= 4; \
        if (counter % 4 == 0) { \
            counter += 7; \
        } else { \
            counter -= 3; \
        } \
    } \
}



#define JUNK_CODE_FUNCTION_2() { \
    int a = 10; \
    for (int i = 0; i < 5; ++i) { \
        a += i * 3; \
        if (a % 2 == 0) { \
            a -= i; \
        } else { \
            a += 2 * i; \
        } \
    } \
    \
    if (a > 20) { \
        for (int j = 0; j < 3; ++j) { \
            a *= (j + 2); \
            if (a % 3 == 0) { \
                a += j; \
            } else { \
                a -= j; \
            } \
        } \
    } else { \
        a -= 5; \
        if (a < 10) { \
            a += 3; \
        } else { \
            a -= 2; \
        } \
    } \
    \
    while (a % 4 != 0) { \
        a += 2; \
        if (a % 5 == 0) { \
            a -= 3; \
        } else { \
            a += 4; \
        } \
    } \
    \
    while (a % 4 != 0) { \
        a += 2; \
        if (a % 3 == 0) { \
            a -= 2; \
        } else { \
            a += 3; \
        } \
    } \
}
#define JUNK_CODE_FUNCTION_3() { \
    int counter = 5; \
    while (counter < 25) { \
        counter += 4; \
        if (counter % 3 == 0) { \
            counter *= 2; \
        } else { \
            counter -= 3; \
        } \
    } \
    \
    if (counter > 15) { \
        for (int i = 0; i < 5; ++i) { \
            counter += i * 6; \
            if (counter % 4 == 0) { \
                counter -= i; \
            } else { \
                counter += 3 * i; \
            } \
        } \
    } else { \
        counter -= 7; \
        if (counter < 10) { \
            counter += 4; \
        } else { \
            counter -= 2; \
        } \
    } \
    \
    do { \
        counter += 3; \
        if (counter % 6 == 0) { \
            counter -= 4; \
        } else { \
            counter += 5; \
        } \
    } while (counter % 5 != 0); \
    \
    while (counter > 0) { \
        counter -= 3; \
        if (counter % 4 == 0) { \
            counter += 6; \
        } else { \
            counter -= 2; \
        } \
    } \
}
#define JUNK_CODE_FUNCTION_4() { \
    int counter = 5; \
    while (counter < 25) { \
        counter += 10; \
        if (counter % 3 == 0) { \
            counter *= 2; \
        } else { \
            counter -= 3; \
        } \
    } \
    \
    if (counter > 15) { \
        for (int i = 0; i < 5; ++i) { \
            counter += i * 6; \
            if (counter % 4 == 0) { \
                counter -= i; \
            } else { \
                counter += 3 * i; \
            } \
        } \
    } else { \
        counter -= 7; \
        if (counter < 10) { \
            counter += 4; \
        } else { \
            counter -= 2; \
        } \
    } \
    \
    do { \
        counter += 3; \
        if (counter % 6 == 0) { \
            counter -= 4; \
        } else { \
            counter += 5; \
        } \
    } while (counter % 5 != 0); \
    \
    while (counter > 0) { \
        counter -= 50; \
        if (counter % 4 == 0) { \
            counter += 6; \
        } else { \
            counter -= 2; \
        } \
    } \
}
#define JUNK_CODE_FUNCTION_5() { \
    int counter = 5; \
    while (counter < 35) { \
        counter += 6; \
        if (counter % 3 == 0) { \
            counter *= 2; \
        } else { \
            counter -= 3; \
        } \
    } \
    \
    if (counter > 15) { \
        for (int i = 0; i < 7; ++i) { \
            counter += i * 8; \
            if (counter % 4 == 0) { \
                counter -= i; \
            } else { \
                counter += 3 * i; \
            } \
        } \
    } else { \
        counter -= 9; \
        if (counter < 10) { \
            counter += 6; \
        } else { \
            counter -= 2; \
        } \
    } \
    \
    do { \
        counter += 5; \
        if (counter % 6 == 0) { \
            counter -= 6; \
        } else { \
            counter += 7; \
        } \
    } while (counter % 5 != 0); \
    \
    while (counter > 0) { \
        counter -= 5; \
        if (counter % 4 == 0) { \
            counter += 8; \
        } else { \
            counter -= 3; \
        } \
    } \
}

#define JUNK_CODE_FUNCTION_6() { \
    int counter = 5; \
    while (counter < 40) { \
        counter += 7; \
        if (counter % 3 == 0) { \
            counter *= 2; \
            if (counter % 5 == 0) { \
                counter -= 3; \
            } else { \
                counter += 4; \
            } \
        } else { \
            counter -= 3; \
        } \
    } \
    \
    if (counter > 15) { \
        for (int i = 0; i < 8; ++i) { \
            counter += i * 9; \
            if (counter % 4 == 0) { \
                counter -= i; \
                if (counter % 3 == 0) { \
                    counter += 2 * i; \
                } else { \
                    counter -= i; \
                } \
            } else { \
                counter += 3 * i; \
            } \
        } \
    } else { \
        counter -= 10; \
        if (counter < 10) { \
            counter += 7; \
            if (counter % 2 == 0) { \
                counter -= 2; \
            } else { \
                counter += 3; \
            } \
        } else { \
            counter -= 2; \
        } \
    } \
    \
    do { \
        counter += 6; \
        if (counter % 6 == 0) { \
            counter -= 5; \
            if (counter % 4 == 0) { \
                counter += 8; \
            } else { \
                counter -= 3; \
            } \
        } else { \
            counter += 7; \
        } \
    } while (counter % 5 != 0); \
    \
    while (counter > 0) { \
        counter -= 6; \
        if (counter % 4 == 0) { \
            counter += 9; \
        } else { \
            counter -= 3; \
            if (counter % 2 == 0) { \
                counter += 4; \
            } else { \
                counter -= 2; \
            } \
        } \
    } \
}

#define JUNK_CODE_SIMPLE_COMPLEXITY() { \
    int a = 10; \
    for (int i = 0; i < 5; ++i) { \
        a += i * 3; \
            bla(); \
    } \
    for (int i = 0; i < 5; ++i) { \
        a += i * 3; \
    } \
    if (a > 20) { \
        for (int j = 0; j < 3; ++j) { \
            a *= (j + 2); \
            bla(); \
        } \
    } \
    if (a > 20) { \
        for (int j = 0; j < 3; ++j) { \
            a *= (j + 2); \
            bla(); \
        } \
    } else { \
        a -= 5; \
    } \
    while (a % 4 != 0) { \
        a += 2; \
    } \
    while (a % 4 != 0) {\
        a += 2; \
            JUNK_CODE_FUNCTION_1(); \
    } \
}

#define JUNK_CODE_MEDIUM_COMPLEXITY() { \
    int a = 10; \
JUNK_CODE_FUNCTION_2(); \
JUNK_CODE_FUNCTION_6(); \
    if (a > 5) { \
        a += 5; \
        if (a < 20) { \
            for (int i = 0; i < 3; ++i) { \
                a += i * 3; \
                if (a % 2 == 0) { \
                    a *= 2; \
                    if (a > 20) { \
                        while (a % 3 != 0) { \
                            a += 3; \
                            for (int j = 0; j < 2; ++j) { \
                                a -= j * 2; \
                                if (a < 15) { \
                                    a += 5; \
                                    if (a % 4 == 0) { \
                                        a *= 2; \
                                        for (int k = 0; k < 3; ++k) { \
                                            a -= k * 3; \
                                            if (a > 10) { \
                                                a /= 2; \
                                                for (int l = 0; l < 4; ++l) { \
                                                    a += l * 4; \
                                                    while (a % 2 != 0) { \
                                                        a -= 1; \
                                                        for (int m = 0; m < 2; ++m) { \
                                                            a *= m + 1; \
                                                            if (a % 3 == 0) { \
                                                                a += 6; \
                                                            } else { \
                                                                a -= 2; \
                                                            } \
                                                            while (a % 4 != 0) { \
                                                                a += 2; \
                                                            } \
                                                        } \
                                                    } \
                                                    if (a < 15) { \
                                                        a += 7; \
                                                        if (a % 3 == 1) { \
                                                            a += 2; \
                                                        } \
                                                    } \
                                                } \
                                            } else { \
                                                a += 4; \
                                                if (a % 5 == 0) { \
                                                    a -= 5; \
                                                } \
                                            } \
                                        } \
                                    } else { \
                                        a -= 3; \
                                    } \
                                } \
                            } \
                        } \
                    } else { \
                        a -= 2; \
                        if (a < 5) { \
                            a += 10; \
                            for (int n = 0; n < 2; ++n) { \
                                a *= n + 1; \
                                if (a % 3 == 0) { \
                                    a += 6; \
                                    for (int o = 0; o < 3; ++o) { \
                                        a -= o * 2; \
                                        while (a % 3 != 0) { \
                                            a += 2; \
                                        } \
                                        if (a < 30) { \
                                            a *= 2; \
                                            for (int p = 0; p < 3; ++p) { \
                                                a -= p * 3; \
                                                if (a > 10) { \
                                                    a /= 2; \
                                                    for (int q = 0; q < 4; ++q) { \
                                                        a += q * 4; \
                                                        while (a % 5 != 0) { \
                                                            a -= 1; \
                                                            JUNK_CODE_FUNCTION_3(); \
                                                            JUNK_CODE_FUNCTION_1(); \
                                                            for (int r = 0; r < 2; ++r) { \
                                                                a += r * 3; \
                                                                if (a % 2 == 1) { \
                                                                    a -= 1; \
                                                                    JUNK_CODE_FUNCTION_6(); \
                                                                    JUNK_CODE_FUNCTION_1(); \
                                                                } \
                                                            } \
                                                        } \
                                                    } \
                                                } else { \
                                                    a += 4; \
                                                                    JUNK_CODE_FUNCTION_2(); \
                                                                    JUNK_CODE_FUNCTION_1(); \
                                                } \
                                            } \
                                        } \
                                    } \
                                } else { \
                                    a -= 2; \
                                } \
                            } \
                        } else { \
                            a -= 5; \
                        } \
                    } \
                } else { \
                    a += 5; \
                    if (a > 25) { \
                        a /= 2; \
                        for (int s = 0; s < 2; ++s) { \
                            a += s * 3; \
                            while (a % 2 != 0) { \
                                a -= 1; \
                            } \
                            if (a > 15) { \
                                a -= 4; \
                                if (a % 5 == 0) { \
                                    a += 5; \
                                    for (int t = 0; t < 2; ++t) { \
                                        a -= t * 2; \
                                        while (a % 3 != 0) { \
                                            a += 2; \
                                        } \
                                        if (a < 30) { \
                                            a *= 2; \
                                            for (int u = 0; u < 3; ++u) { \
                                                a -= u * 3; \
                                                if (a > 10) { \
                                                    a /= 2; \
                                                    for (int v = 0; v < 4; ++v) { \
                                                        a += v * 4; \
                                                        while (a % 5 != 0) { \
                                                            a -= 1; \
                                                            JUNK_CODE_FUNCTION_1(); \
                                                            JUNK_CODE_FUNCTION_1(); \
                                                            JUNK_CODE_FUNCTION_1(); \
                                                        } \
                                                    } \
                                                } else { \
                                                    a += 4; \
                                                } \
                                            } \
                                        } \
                                    } \
                                } \
                            } else { \
                                a += 2; \
                            } \
                        } \
                    } else { \
                        a *= 2; \
                                                            JUNK_CODE_FUNCTION_3(); \
                                                            JUNK_CODE_FUNCTION_1(); \
                                                            JUNK_CODE_FUNCTION_5(); \
                    } \
                } \
            } \
        } \
    } \
}


#define JUNK_CODE_HARD_COMPLEXITY() \
{ \
        int N = 5; \
        if (N % 2 == 0) { \
            for (int u = 0; u < N; ++u) { \
            int randomVar6 = 51; \
            for (int v = 0; v < 6; ++v) { \
                int w = v + 2; \
                do { \
                    w += 3; \
                    if (w == 11) { \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_1(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_2(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                        int innerVar4 = w * randomVar6 / 2; \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_3(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_4(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    } \
                } while (w < 15); \
            } \
            \
            if (randomVar6 > 25) { \
                int x = randomVar6 / 4; \
                do { \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_5(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_4(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    x -= 2; \
                } while (x > 0); \
            } \
            \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_2(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_3(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
        } \
        } else { \
            for (int y = 0; y < N; ++y) { \
            int randomVar2 = 68; \
            for (int z = 0; z < 7; ++z) { \
                int a = z + 4; \
                do { \
                    a += 4; \
                    if (a == 16) { \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_6(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_1(); \
                                JUNK_CODE_FUNCTION_2(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                        int innerVar2 = a * randomVar2 / 3; \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_5(); \
                                    JUNK_CODE_FUNCTION_6(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_3(); \
                                JUNK_CODE_FUNCTION_5(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    } \
                } while (a < 20); \
            } \
            \
            if (randomVar2 > 40) { \
                int b = randomVar2 / 5; \
                do { \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_3(); \
                                    JUNK_CODE_FUNCTION_5(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_6(); \
                                JUNK_CODE_FUNCTION_2(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    b -= 3; \
                } while (b > 0); \
            } \
            \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_5(); \
                                    JUNK_CODE_FUNCTION_4(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_1(); \
                                JUNK_CODE_FUNCTION_3(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    JUNK_CODE_FUNCTION_1();\
                    JUNK_CODE_FUNCTION_5();\
                    JUNK_CODE_FUNCTION_6();\
        } \
}\
}

#define JUNK_CODE_SUPERHARD_COMPLEXITY() \
{ \
        int N = 6; \
if (N % 2 == 0) { \
            for (int q = 0; q < N; ++q) { \
            int randomVar4 = 77; \
            for (int p = 0; p < 8; ++p) { \
                int t = p + 6; \
                do { \
                    t += 5; \
                    if (t == 18) { \
                        JUNK_CODE_FUNCTION_1(); \
                        int innerVar1 = t * randomVar4 / 3; \
                        for (int q = 0; q < N; ++q) { \
                            int randomVar4 = 77; \
                            for (int p = 0; p < 6; ++p) { \
                                int t = p + 3; \
                                do { \
                                    t += 2; \
                                    if (t == 11) { \
                                        JUNK_CODE_FUNCTION_1(); \
                                        int innerVar1 = t * randomVar4 / 3; \
                                    } \
                                } while (t < 18); \
                            } \
                            if (randomVar4 > 35) { \
                                int r = randomVar4 / 5; \
                                do { \
                                    JUNK_CODE_FUNCTION_1(); \
                                    r -= 4; \
                                } while (r > 0); \
                            } \
                        } \
                    } \
                } while (t < 30); \
            } \
            \
            if (randomVar4 > 35) { \
                int r = randomVar4 / 5; \
                do { \
                                for (int p = 0; p < 8; ++p) { \
                int t = p + 6; \
                do { \
                    t += 5; \
                    if (t == 18) { \
                        JUNK_CODE_FUNCTION_1(); \
                        int innerVar1 = t * randomVar4 / 3; \
                        for (int q = 0; q < N; ++q) { \
                            int randomVar4 = 77; \
                            for (int p = 0; p < 6; ++p) { \
                                int t = p + 3; \
                                do { \
                                    t += 2; \
                                    if (t == 11) { \
                                        JUNK_CODE_FUNCTION_1(); \
                                        int innerVar1 = t * randomVar4 / 3; \
                                    } \
                                } while (t < 18); \
                            } \
                            if (randomVar4 > 35) { \
                                int r = randomVar4 / 5; \
                                do { \
                                    JUNK_CODE_FUNCTION_1(); \
                                    r -= 4; \
                                } while (r > 0); \
                            } \
                        } \
                    } \
                } while (t < 30); \
            } \
                    r -= 4; \
                } while (r > 0); \
            } \
            \
                for (int p = 0; p < 8; ++p) { \
                int t = p + 6; \
                do { \
                    t += 5; \
                    if (t == 18) { \
                        JUNK_CODE_FUNCTION_1(); \
                        int innerVar1 = t * randomVar4 / 3; \
                        for (int q = 0; q < N; ++q) { \
                            int randomVar4 = 77; \
                            for (int p = 0; p < 6; ++p) { \
                                int t = p + 3; \
                                do { \
                                    t += 2; \
                                    if (t == 11) { \
                                        JUNK_CODE_FUNCTION_1(); \
                                        int innerVar1 = t * randomVar4 / 3; \
                                    } \
                                } while (t < 18); \
                            } \
                            if (randomVar4 > 35) { \
                                int r = randomVar4 / 5; \
                                do { \
                                    JUNK_CODE_FUNCTION_1(); \
                                    r -= 4; \
                                } while (r > 0); \
                            } \
                        } \
                    } \
                } while (t < 30); \
            } \
        } \
        } else { \
            for (int q = 0; q < N; ++q) { \
            int randomVar4 = 89; \
            for (int p = 0; p < 7; ++p) { \
                int t = p + 4; \
                do { \
                    t += 3; \
                    if (t == 14) { \
                        JUNK_CODE_FUNCTION_1(); \
                        int innerVar1 = t * randomVar4 / 2; \
                        JUNK_CODE_FUNCTION_1(); \
                    } \
                } while (t < 25); \
            } \
            \
            if (randomVar4 > 40) { \
                int r = randomVar4 / 6; \
                do { \
                    JUNK_CODE_FUNCTION_1(); \
                    r -= 3; \
                } while (r > 0); \
            } \
            \
                    JUNK_CODE_FUNCTION_1(); \
        } \
        } \
        if (N % 2 == 0) { \
            for (int u = 0; u < N; ++u) { \
            int randomVar6 = 51; \
            for (int v = 0; v < 6; ++v) { \
                int w = v + 2; \
                do { \
                    w += 3; \
                    if (w == 11) { \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_1(); \
                                    JUNK_CODE_FUNCTION_1(); \
                                    JUNK_CODE_FUNCTION_1(); \
                                    JUNK_CODE_FUNCTION_1(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_2(); \
                                JUNK_CODE_FUNCTION_2(); \
                                JUNK_CODE_FUNCTION_2(); \
                                JUNK_CODE_FUNCTION_2(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                        int innerVar4 = w * randomVar6 / 2; \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_3(); \
                                    JUNK_CODE_FUNCTION_3(); \
                                    JUNK_CODE_FUNCTION_3(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_4(); \
                                JUNK_CODE_FUNCTION_4(); \
                                JUNK_CODE_FUNCTION_4(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    } \
                } while (w < 15); \
            } \
            \
            if (randomVar6 > 25) { \
                int x = randomVar6 / 4; \
                do { \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_5(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_4(); \
                                JUNK_CODE_FUNCTION_4(); \
                                JUNK_CODE_FUNCTION_4(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    x -= 2; \
                } while (x > 0); \
            } \
            \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_2(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_3(); \
                                JUNK_CODE_FUNCTION_3(); \
                                JUNK_CODE_FUNCTION_3(); \
                                JUNK_CODE_FUNCTION_3(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
        } \
        } else { \
            for (int y = 0; y < N; ++y) { \
            int randomVar2 = 68; \
            for (int z = 0; z < 7; ++z) { \
                int a = z + 4; \
                do { \
                    a += 4; \
                    if (a == 16) { \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_6(); \
                                    JUNK_CODE_FUNCTION_6(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_1(); \
                                JUNK_CODE_FUNCTION_2(); \
                                JUNK_CODE_FUNCTION_2(); \
                                JUNK_CODE_FUNCTION_2(); \
                                JUNK_CODE_FUNCTION_2(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                        int innerVar2 = a * randomVar2 / 3; \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_5(); \
                                    JUNK_CODE_FUNCTION_6(); \
                                    JUNK_CODE_FUNCTION_6(); \
                                    JUNK_CODE_FUNCTION_6(); \
                                    JUNK_CODE_FUNCTION_6(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_3(); \
                                JUNK_CODE_FUNCTION_3(); \
                                JUNK_CODE_FUNCTION_3(); \
                                JUNK_CODE_FUNCTION_5(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    } \
                } while (a < 20); \
            } \
            \
            if (randomVar2 > 40) { \
                int b = randomVar2 / 5; \
                do { \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_3(); \
                                    JUNK_CODE_FUNCTION_5(); \
                                    JUNK_CODE_FUNCTION_5(); \
                                    JUNK_CODE_FUNCTION_5(); \
                                    JUNK_CODE_FUNCTION_5(); \
                                    JUNK_CODE_FUNCTION_5(); \
                                    JUNK_CODE_FUNCTION_5(); \
                                    JUNK_CODE_FUNCTION_5(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_6(); \
                                JUNK_CODE_FUNCTION_2(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    b -= 3; \
                } while (b > 0); \
            } \
            \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_5(); \
                                    JUNK_CODE_FUNCTION_4(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_1(); \
                                JUNK_CODE_FUNCTION_3(); \
                                JUNK_CODE_FUNCTION_3(); \
                                JUNK_CODE_FUNCTION_3(); \
                                JUNK_CODE_FUNCTION_3(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
        } \
}\
if (N % 2 == 0) { \
            for (int q = 0; q < N; ++q) { \
            int randomVar4 = 77; \
            for (int p = 0; p < 8; ++p) { \
                int t = p + 6; \
                do { \
                    t += 5; \
                    if (t == 18) { \
                        JUNK_CODE_FUNCTION_1(); \
                        int innerVar1 = t * randomVar4 / 3; \
                        for (int q = 0; q < N; ++q) { \
                            int randomVar4 = 77; \
                            for (int p = 0; p < 6; ++p) { \
                                int t = p + 3; \
                                do { \
                                    t += 2; \
                                    if (t == 11) { \
                                        JUNK_CODE_FUNCTION_1(); \
                                        int innerVar1 = t * randomVar4 / 3; \
                                    } \
                                } while (t < 18); \
                            } \
                            if (randomVar4 > 35) { \
                                int r = randomVar4 / 5; \
                                do { \
                                    JUNK_CODE_FUNCTION_1(); \
                                    r -= 4; \
                                } while (r > 0); \
                            } \
                        } \
                    } \
                } while (t < 30); \
            } \
            \
            if (randomVar4 > 35) { \
                int r = randomVar4 / 5; \
                do { \
                                for (int p = 0; p < 8; ++p) { \
                int t = p + 6; \
                do { \
                    t += 5; \
                    if (t == 18) { \
                        JUNK_CODE_FUNCTION_1(); \
                        int innerVar1 = t * randomVar4 / 3; \
                        for (int q = 0; q < N; ++q) { \
                            int randomVar4 = 77; \
                            for (int p = 0; p < 6; ++p) { \
                                int t = p + 3; \
                                do { \
                                    t += 2; \
                                    if (t == 11) { \
                                        JUNK_CODE_FUNCTION_1(); \
                                        int innerVar1 = t * randomVar4 / 3; \
                                    } \
                                } while (t < 18); \
                            } \
                            if (randomVar4 > 35) { \
                                int r = randomVar4 / 5; \
                                do { \
                                    JUNK_CODE_FUNCTION_1(); \
                                    r -= 4; \
                                } while (r > 0); \
                            } \
                        } \
                    } \
                } while (t < 30); \
            } \
                    r -= 4; \
                } while (r > 0); \
            } \
            \
                for (int p = 0; p < 8; ++p) { \
                int t = p + 6; \
                do { \
                    t += 5; \
                    if (t == 18) { \
                        JUNK_CODE_FUNCTION_1(); \
                        int innerVar1 = t * randomVar4 / 3; \
                        for (int q = 0; q < N; ++q) { \
                            int randomVar4 = 77; \
                            for (int p = 0; p < 6; ++p) { \
                                int t = p + 3; \
                                do { \
                                    t += 2; \
                                    if (t == 11) { \
                                        JUNK_CODE_FUNCTION_1(); \
                                        int innerVar1 = t * randomVar4 / 3; \
                                    } \
                                } while (t < 18); \
                            } \
                            if (randomVar4 > 35) { \
                                int r = randomVar4 / 5; \
                                do { \
                                    JUNK_CODE_FUNCTION_1(); \
                                    r -= 4; \
                                } while (r > 0); \
                            } \
                        } \
                    } \
                } while (t < 30); \
            } \
        } \
        } else { \
            for (int q = 0; q < N; ++q) { \
            int randomVar4 = 89; \
            for (int p = 0; p < 7; ++p) { \
                int t = p + 4; \
                do { \
                    t += 3; \
                    if (t == 14) { \
                        JUNK_CODE_FUNCTION_1(); \
                        int innerVar1 = t * randomVar4 / 2; \
                        JUNK_CODE_FUNCTION_1(); \
                    } \
                } while (t < 25); \
            } \
            \
            if (randomVar4 > 40) { \
                int r = randomVar4 / 6; \
                do { \
                    JUNK_CODE_FUNCTION_1(); \
                    r -= 3; \
                } while (r > 0); \
            } \
            \
                    JUNK_CODE_FUNCTION_1(); \
        } \
        } \
        if (N % 2 == 0) { \
            for (int u = 0; u < N; ++u) { \
            int randomVar6 = 51; \
            for (int v = 0; v < 6; ++v) { \
                int w = v + 2; \
                do { \
                    w += 3; \
                    if (w == 11) { \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_1(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_2(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                        int innerVar4 = w * randomVar6 / 2; \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_3(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_4(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    } \
                } while (w < 15); \
            } \
            \
            if (randomVar6 > 25) { \
                int x = randomVar6 / 4; \
                do { \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_4(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_4(); \
                                JUNK_CODE_FUNCTION_4(); \
                                JUNK_CODE_FUNCTION_4(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    x -= 2; \
                } while (x > 0); \
            } \
            \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_2(); \
                                    JUNK_CODE_FUNCTION_2(); \
                                    JUNK_CODE_FUNCTION_2(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_3(); \
                                JUNK_CODE_FUNCTION_3(); \
                                JUNK_CODE_FUNCTION_3(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
        } \
        } else { \
            for (int y = 0; y < N; ++y) { \
            int randomVar2 = 68; \
            for (int z = 0; z < 7; ++z) { \
                int a = z + 4; \
                do { \
                    a += 4; \
                    if (a == 16) { \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_6(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_1(); \
                                JUNK_CODE_FUNCTION_1(); \
                                JUNK_CODE_FUNCTION_1(); \
                                JUNK_CODE_FUNCTION_2(); \
                                JUNK_CODE_FUNCTION_2(); \
                                JUNK_CODE_FUNCTION_2(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                        int innerVar2 = a * randomVar2 / 3; \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_2(); \
                                    JUNK_CODE_FUNCTION_6(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_1(); \
                                JUNK_CODE_FUNCTION_5(); \
                                JUNK_CODE_FUNCTION_5(); \
                                JUNK_CODE_FUNCTION_5(); \
                                JUNK_CODE_FUNCTION_5(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    } \
                } while (a < 20); \
            } \
            \
            if (randomVar2 > 40) { \
                int b = randomVar2 / 5; \
                do { \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_4(); \
                                    JUNK_CODE_FUNCTION_5(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_6(); \
                                JUNK_CODE_FUNCTION_6(); \
                                JUNK_CODE_FUNCTION_6(); \
                                JUNK_CODE_FUNCTION_5(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    b -= 3; \
                } while (b > 0); \
            } \
            \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_5(); \
                                    JUNK_CODE_FUNCTION_4(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_5(); \
                                JUNK_CODE_FUNCTION_3(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
        } \
}\
        if (N % 2 == 0) { \
            for (int u = 0; u < N; ++u) { \
            int randomVar6 = 51; \
            for (int v = 0; v < 6; ++v) { \
                int w = v + 2; \
                do { \
                    w += 3; \
                    if (w == 11) { \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_1(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_1(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                        int innerVar4 = w * randomVar6 / 2; \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_2(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_4(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    } \
                } while (w < 15); \
            } \
            \
            if (randomVar6 > 25) { \
                int x = randomVar6 / 4; \
                do { \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_5(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_3(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    x -= 2; \
                } while (x > 0); \
            } \
            \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_5(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_3(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
        } \
        } else { \
            for (int y = 0; y < N; ++y) { \
            int randomVar2 = 68; \
            for (int z = 0; z < 7; ++z) { \
                int a = z + 4; \
                do { \
                    a += 4; \
                    if (a == 16) { \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_1(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_1(); \
                                JUNK_CODE_FUNCTION_2(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                        int innerVar2 = a * randomVar2 / 3; \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_5(); \
                                    JUNK_CODE_FUNCTION_2(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_3(); \
                                JUNK_CODE_FUNCTION_3(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    } \
                } while (a < 20); \
            } \
            \
            if (randomVar2 > 40) { \
                int b = randomVar2 / 5; \
                do { \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_3(); \
                                    JUNK_CODE_FUNCTION_6(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_6(); \
                                JUNK_CODE_FUNCTION_2(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
                    b -= 3; \
                } while (b > 0); \
            } \
            \
                    for (int q = 0; q < N; ++q) { \
                        int randomVar4 = 7; \
                        for (int p = 0; p < 2; ++p) { \
                            int t = p + 1; \
                            do { \
                                t += 1; \
                                if (t == 2) { \
                                    JUNK_CODE_FUNCTION_5(); \
                                    JUNK_CODE_FUNCTION_1(); \
                                    int innerVar1 = t * randomVar4 / 2; \
                                } \
                            } while (t < 3); \
                        } \
                        if (randomVar4 > 5) { \
                            int r = randomVar4 / 2; \
                            do { \
                                JUNK_CODE_FUNCTION_1(); \
                                JUNK_CODE_FUNCTION_2(); \
                                r -= 1; \
                            } while (r > 0); \
                        } \
                    } \
        } \
}\
}

//static int number = 5; // Default value
//void setNumber(int newNumber);
//int getNumber();

//VOID Authorize(std::string);
VOID AntiDebugging();
//VOID BSOD();
//VOID AntiHook();
//VOID hookbsod();
//VOID hookmsgboxa();
//VOID unhookbsod();
//VOID unhookmsgboxa();
VOID Anti64();
VOID AntiX64();
VOID AntiPH();
VOID AntiCE();
VOID AntiIDA();
VOID AntiDH();
//VOID AntiCMD();
//VOID NOSUSPEND();
//VOID ProtectProcess();
//VOID RefreshModules();
//VOID AntiInjection();
//VOID CheckText();
//VOID MemoryCheck();
//VOID AntiDump();
//VOID CheckSize(DWORD min, DWORD max);
VOID JunkCode();
VOID banning();
//VOID Check();
VOID SearchWindowClassNames();
//VOID AntiDissam();
//VOID AntiCallbackThread();
//VOID AntiCallbackProcess();
//VOID AntiBuffer();
//VOID ExtraAntiDebugging();
//VOID AntiObject();
//VOID RefreshModules();
//VOID AntiInjection();
VOID CheckBan();
//VOID AntiSuspend();
//VOID KernelAntiDebugging();
//VOID CloseKernelProtection();
//VOID SendToLogs(std::string log, LPCWSTR  webhook);
//bool IsSuspiciousMemoryAccessDetected(const void* targetAddress, const size_t bufferSize);
//std::string CheckSub(std::string AppName, std::string AppID, std::string AppSecret, std::string AppVersion, std::string AppUrl, std::string License);
//VOID CheckSesh(std::string AppName, std::string AppID, std::string AppSecret, std::string AppVersion, std::string AppUrl, std::string License);
//bool KeyAuthOnRoids(std::string AppName, std::string AppID, std::string AppSecret, std::string AppVersion, std::string AppUrl, std::string License);
//std::vector<std::uint8_t> InstallBytes(std::string AppName, std::string AppID, std::string AppSecret, std::string AppVersion, std::string AppUrl, std::string dwnloadkey, std::string License);
//std::vector<std::uint8_t> InstallEncryptedBytes(std::string AppName, std::string AppID, std::string AppSecret, std::string AppVersion, std::string AppUrl, std::string dwnloadkey, std::string License, std::vector<unsigned char> encryptionKey);
//VOID EncryptData(std::vector<std::uint8_t> bytes, std::vector<unsigned char> encryptionKey);
//VOID CheckIntegOfEncryption();
//VOID StartEncryption();


inline std::thread a;
inline std::thread b;
inline std::thread c;
inline std::thread d;
inline std::thread e;
inline std::thread f;
inline std::thread g;
inline std::thread h;

inline void InitThreads() {

    a = std::thread{ AntiDebugging };
    b = std::thread{ SearchWindowClassNames };
  //  c = std::thread{ Anti64 };
 //   d = std::thread{ AntiIDA };
 //   e = std::thread{ AntiX64 };
//   f = std::thread{ AntiPH };
 //    g = std::thread{ AntiCE };
 //  h = std::thread{ AntiDH };


}



inline void threadstest() {
    AntiDebugging();
    SearchWindowClassNames();
    Anti64();
    AntiIDA();
    AntiX64();
    AntiPH();
    AntiCE();
    AntiDH();

}


inline void ThreadCheck() {
    if (!a.joinable()) {
        exit(0);
    }
    if (!b.joinable()) {
        exit(0);
    }
 /*   if (!c.joinable()) {
        exit(0);
    }
    if (!d.joinable()) {
        exit(0);
    }*/
   /* if (!e.joinable()) {
        exit(0);
    }*/
 /*   if (!f.joinable()) {
        exit(0);
    }
    if (!g.joinable()) {
        exit(0);
    }
    if (!h.joinable()) {
        exit(0);
    }*/
}

#define SUPERHARDCOMPLEXITY() JUNK_CODE_HARD_COMPLEXITY()
#define VL_OBFUSCATION_BEGIN
#define VL_OBFUSCATION_END
//
//#define PROTECT(func) \
//    do { \
//        const void* funcAddress = reinterpret_cast<const void*>(&(func)); \
//        if (IsSuspiciousMemoryAccessDetected(funcAddress, 1)) { \
//            BSOD(); \
//        } \
//        else { \
//            (func)(); \
//        } \
//    } while (0)


#include <array>

template <typename T, SIZE_T N, SIZE_T K>
class DIAtring
{
private:
    std::array<T, N> str_;

    __forceinline constexpr T Xor(T c) const
    {
        return (T)(c ^ K);
    }

public:
    template <SIZE_T... IS>
    __forceinline constexpr DIAtring(const T* str, std::index_sequence<IS...>) : str_{ Xor(str[IS])... } {};

    template <SIZE_T... IS>
    __forceinline const T* Get(std::index_sequence<IS...>)
    {
        str_ = { Xor(str_[IS])... };

        return str_.data();
    }
};

#define MIS(s) (std::make_index_sequence<ARRAYSIZE(s)>())
#define DIA(s) ([]{ constexpr DIAtring<std::decay<decltype(*s)>::type, ARRAYSIZE(s), __LINE__> t(s, MIS(s)); return t; }().Get(MIS(s)))