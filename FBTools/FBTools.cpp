#include "Plugin.h"


#include <kernwin.hpp>


#include "ActionManager.h"


#include "Actions/FindTypeInfo.h"
#include "Actions/RenameTypeInfo.h"

#include "Actions/RenameMessages.h"
#include "Actions/FindMessageHandlers.h"

#include "Actions/DumpDefinitions.h"

void RegisterMenuActions( )
{
	if ( !create_menu( "FBTools", "FBTools" ) )
	{
		msg( "Failed to create fbtoools menu!\n" );
		return;
	}

	auto& Manager = Action::ActionManager::GetInstance( );

	Manager.RegisterAction( new Actions::FindTypeInfoAction( [&]( Frostbite::FBVersion type, ea_t typeInfo )
	{
		Manager.RegisterAction( new Actions::RenameTypeInfo( type ) );
		Manager.RegisterAction( new Actions::RenameMessages( type ) );
		Manager.RegisterAction( new Actions::FindMessageHandlers( type ) );
		Manager.RegisterAction( new Actions::DumpDefinitions( type ) );

	} ) );
	
}




bool idaapi IDAP_run( size_t arg )
{
	msg( "[+] Doing the stuff\n" );

	//InitGUI( );

	// msg( "[+] Unloading FBTools plugin.\n" );
	//PLUGIN.flags |= PLUGIN_UNL;

	return true;
}

void idaapi IDAP_term( void )
{
	msg( "[+] IDAP_term\n" );
	return;
}

int idaapi IDAP_init( void )
{
	if ( !is_idaq( ) )
		return PLUGIN_SKIP;


	msg( "[+] Loading FBTools 3!\n" );
	RegisterMenuActions( );

	msg( "[+] FBTools 3 Loaded\n" );

	return PLUGIN_KEEP;
}
