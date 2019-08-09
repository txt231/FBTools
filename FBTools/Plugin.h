#pragma once

#include <ida.hpp>
#include <idp.hpp>
#include <loader.hpp>
#include <kernwin.hpp>

char IDAP_comment[] = "Comment for FBTools 3\0";
char IDAP_help[] = "Help for this tool for Frostbite\0";
char IDAP_name[] = "FBTools\0";
char IDAP_hotkey[] = "\0";


int idaapi IDAP_init( );

void idaapi IDAP_term( );

bool idaapi IDAP_run( size_t arg );


plugin_t PLUGIN =
{
	IDP_INTERFACE_VERSION,
	PLUGIN_FIX,//0,
	&IDAP_init, &IDAP_term, &IDAP_run,
	IDAP_comment, IDAP_help, IDAP_name, nullptr
};
