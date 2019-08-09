#pragma once


#include <string>

#include "Plugin.h"

#include <kernwin.hpp>

namespace Action
{

	class ActionBase
		: public action_handler_t
	{
	public:
		ActionBase( std::string name,
					std::string label,
					std::string tooltip,
					std::string shortcut = "" )
			: m_Name( name )
			, m_Label( label )
			, m_Tooltip( tooltip )
			, m_Shortcut( shortcut )
		{ }
		std::string m_Name;
		std::string m_Label;
		std::string m_Tooltip;
		std::string m_Shortcut;





		void RegisterAction( )
		{
			msg( "[+] Registering action %s\n", this->m_Name.c_str( ) );


			action_desc_t Action = ACTION_DESC_LITERAL( m_Name.c_str( ),
														m_Label.c_str( ),
														this,
														m_Shortcut.size( ) == 0 ? nullptr : m_Shortcut.c_str( ),
														m_Tooltip.size( ) == 0 ? nullptr : m_Tooltip.c_str( ),
														-1 );


			if ( !register_action( Action ) )
			{
				msg( "[!] Failed to register action!\n" );

				return;
			}

			msg( "[+] Attaching action!\n" );

			if ( !attach_action_to_menu( "FBTools", m_Name.c_str( ), SETMENU_APP ) )
			{
				msg( "[!] Failed to add action!\n" );

				return;
			}
		}
	};

}