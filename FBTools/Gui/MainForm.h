#ifndef MAINFORM_H
#define MAINFORM_H

#include <windows.h>
#include <kernwin.hpp>
#include <vector>
#include <tchar.h>

HWND GetIDAWindow( void )
{
	// return reinterpret_cast<HWND>( reinterpret_cast<__int64>( callui( ui_get_hwnd ).vptr ) & 0xffffffff );
	return reinterpret_cast<HWND>( callui( ui_get_hwnd ).vptr );
}


#endif