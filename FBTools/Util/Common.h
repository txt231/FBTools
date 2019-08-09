#pragma once

#include "windows.h"

namespace util
{
	HMODULE GetCurrentModule( )
	{
		HMODULE hModule = NULL;
		GetModuleHandleEx( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<LPCTSTR>( GetCurrentModule ), &hModule );

		return hModule;
	}
}

