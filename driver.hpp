#pragma once

#include <Windows.h>
#include <cstdint>
#include <TlHelp32.h>

typedef struct _IO_STATUS_BLOCK {
	union {
		NTSTATUS Status;
		PVOID    Pointer;
	};
	ULONG_PTR Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;

/* typedefs */

typedef
VOID
(NTAPI* PIO_APC_ROUTINE) (
	IN PVOID ApcContext,
	IN PIO_STATUS_BLOCK IoStatusBlock,
	IN ULONG Reserved
	);

extern "C" __int64 DirectIO(
	HANDLE FileHandle,
	HANDLE Event,
	PIO_APC_ROUTINE
	ApcRoutine,
	PVOID ApcContext,
	PIO_STATUS_BLOCK IoStatusBlock,
	ULONG IoControlCode,
	PVOID InputBuffer,
	ULONG InputBufferLength,
	PVOID OutputBuffer,
	ULONG OutputBufferLength);

#define emulate inline

#define code_read_physical CTL_CODE(FILE_DEVICE_UNKNOWN, 0x366, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_read_virtual CTL_CODE(FILE_DEVICE_UNKNOWN, 0x903, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_write CTL_CODE(FILE_DEVICE_UNKNOWN, 0x431, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_get_base_address CTL_CODE(FILE_DEVICE_UNKNOWN, 0x136, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_decrypt_cr3 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x236, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_restart_driver CTL_CODE(FILE_DEVICE_UNKNOWN, 0x368, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _rw 
{
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
} rw, * WriteStruct;

typedef struct _r 
{
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
} rwr, * ReadStruct;

typedef struct _ba 
{
	INT32 process_id;
	ULONGLONG* address;
} ba, * BaseAddressStruct;

typedef struct _cr3 
{
	INT32 process_id;
} cr3, * DTBStruct;

typedef struct _restart 
{
	bool bResetCr3;
} restart, * RestartStruct;

typedef struct _movemouse 
{
	long x;
	long y;
	unsigned short button_flags;
} movemouse, * MouseMovementStruct;

typedef struct _hf 
{
	INT32 process_id;
} hidefile, * HideFileStruct;

namespace Bypass
{
	emulate HANDLE dHandle;
	emulate INT32 ProcID;
	emulate uintptr_t GameBase;


	emulate bool VirtualMode;

	emulate bool SetupDriver( );

	emulate bool read( PVOID address, PVOID buffer, DWORD size );

	emulate bool write_physical( PVOID address, PVOID buffer, DWORD size );

	emulate uintptr_t get_base_address( );

	emulate uintptr_t Attach( LPCTSTR process_name );

	emulate void bShouldUseVirtualMode( bool mode );

	emulate bool BypassControlRegister3( );

	emulate bool RestartBuffers( );


}

class Kern
{
public:
	template <typename T>
	inline T read( uint64_t address ) 
	{
		T buffer{ };
		Bypass::read( (PVOID)address, &buffer, sizeof( T ) );
		return buffer;
	}

	template <typename T>
	inline T write( uint64_t address, T buffer ) 
	{
		Bypass::write_physical( (PVOID)address, &buffer, sizeof( T ) );
		return buffer;
	}

	template<typename T>
	bool read_large_array( uint64_t address, T out[ ], size_t len )
	{
		size_t real_size = sizeof( T ) * len;
		size_t read_size = 0;
		T* temp = out;  // Temporary pointer to keep track of the current position in the out array

		while ( read_size < real_size ) {
			BYTE* buffer = new BYTE[ 512 ];

			size_t diff = real_size - read_size;
			if ( diff > 512 )
				diff = 512;

			Bypass::read( ( PVOID )( address + read_size ), buffer, diff );

			memcpy( temp, buffer, diff );

			read_size += diff;
			temp += ( diff / sizeof( T ) );

			delete[ ] buffer;
		}

		return true;
	}

	inline std::string read_ascii( uint64_t adr )
	{
		char buf[ 64 ] = { 0 };
		Bypass::read( (PVOID)adr, &buf, 64 );
		return buf;
	}

	template<typename T>
	inline bool read_array( uint64_t address, T out[ ], size_t len )
	{
		return Bypass::read( (PVOID)address, ( uint8_t* )out, sizeof( T ) * len ) == 0;
	}

	bool IsValidAddress( const uint64_t address ) {
		if ( address <= 0x400000 || address == 0xCCCCCCCCCCCCCCCC || reinterpret_cast< void* >( address ) == nullptr || address >
			0x7FFFFFFFFFFFFFFF ) {
			return false;
		}

		return true;
	}

}; emulate Kern* request;



inline void Bypass::bShouldUseVirtualMode( bool mode )
{
	VirtualMode = mode;
}

inline uintptr_t Bypass::get_base_address( ) 
{
	IO_STATUS_BLOCK block;

	uintptr_t image_address = { NULL };
	_ba arguments = { NULL };

	arguments.process_id = ProcID;
	arguments.address = (ULONGLONG*)&image_address;

	DirectIO(dHandle, nullptr, nullptr, nullptr, &block, code_get_base_address, &arguments, sizeof(arguments), &arguments, sizeof(arguments));

	return image_address;
}

inline bool Bypass::write_physical( PVOID address, PVOID buffer, DWORD size ) 
{
	IO_STATUS_BLOCK block;

	_rw arguments = { 0 };

	arguments.address = (ULONGLONG)address;
	arguments.buffer = (ULONGLONG)buffer;
	arguments.size = size;
	arguments.process_id = ProcID;

	return 	DirectIO(dHandle, nullptr, nullptr, nullptr, &block, code_write, &arguments, sizeof(arguments), &arguments, sizeof(arguments));
}

/*
inline bool Bypass::KernelMouseMovement( int x, int y ) 
{
	_movemouse arguments = { 0 };

	arguments.x = (long)y;
	arguments.y = (long)x;

	return DeviceIoControl( dHandle, code_MoveMouse, &arguments, sizeof( arguments ), nullptr, NULL, NULL, NULL );
}
*/

inline bool Bypass::read( PVOID address, PVOID buffer, DWORD size ) 
{
	IO_STATUS_BLOCK block;

	_r arguments = { 0 };
	arguments.address = (ULONGLONG)address;
	arguments.buffer = (ULONGLONG)buffer;
	arguments.size = size;
	arguments.process_id = ProcID;

	if (VirtualMode)
	{
		return DirectIO(dHandle, nullptr, nullptr, nullptr, &block, code_read_virtual, &arguments, sizeof(arguments), &arguments, sizeof(arguments));
	}
	else {
		return DirectIO(dHandle, nullptr, nullptr, nullptr, &block, code_read_physical, &arguments, sizeof(arguments), &arguments, sizeof(arguments));
	}
}

inline uintptr_t Bypass::Attach( LPCTSTR process_name ) 
{
	PROCESSENTRY32 pt;
	HANDLE hsnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	pt.dwSize = sizeof( PROCESSENTRY32 );
	if ( Process32First( hsnap, &pt ) ) {
		do  {
			if ( !lstrcmpi( pt.szExeFile, process_name ) )  {
				CloseHandle( hsnap );
				ProcID = pt.th32ProcessID;
				return pt.th32ProcessID;
			}
		} while ( Process32Next( hsnap, &pt ) );
	}
	CloseHandle( hsnap );

	return { NULL };
}

inline bool Bypass::SetupDriver( ) 
{
	auto DriverName = SK( "\\\\.\\HwNClx0101" );

	dHandle = CreateFileA( DriverName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );

	if ( !dHandle || ( dHandle == INVALID_HANDLE_VALUE ) )
		return false;

	return true;
}

inline bool Bypass::BypassControlRegister3( )
{
	IO_STATUS_BLOCK block;

	_cr3 arguments = { NULL };

	arguments.process_id = ProcID;

	return DirectIO(dHandle, nullptr, nullptr, nullptr, &block, code_decrypt_cr3, &arguments, sizeof(arguments), &arguments, sizeof(arguments));
}

inline bool Bypass::RestartBuffers( ) 
{
	IO_STATUS_BLOCK block;

	_restart arguments = { NULL };

	arguments.bResetCr3 = true;

	return DirectIO(dHandle, nullptr, nullptr, nullptr, &block, code_restart_driver, &arguments, sizeof(arguments), &arguments, sizeof(arguments));
}
