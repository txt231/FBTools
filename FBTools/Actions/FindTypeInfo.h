#pragma once

#include <functional>

#include "../ActionBase.h"

#include "../Core/Pattern.h"

#include "../Frostbite/FrostbiteUtil.h"
//#include "../Frostbite/FBType.h"

namespace Actions
{

	static std::map<Frostbite::FBType, std::string> c_FbSigs =
	{
		std::pair<Frostbite::FBType, std::string>( Frostbite::FBT_Fb2014,
												   "48 C7 43 ?? ?? ?? ?? ??"
												   "48 8B 05 [ ?? ?? ?? ?? ]"
												   "48 89 43 08" ),
	};

	class FindTypeInfoAction
		: public Action::ActionBase
	{
	public:
		FindTypeInfoAction( std::function<void( Frostbite::FBType, ea_t )> callback )
			: ActionBase( "FBTools:FindTypeInfo",
						  "Find FirstTypeInfo",
						  "Finds FirstTypeInfo so you can use other actions..." )
			, m_Callback( callback )
		{ }

		std::function<void( Frostbite::FBType, ea_t )> m_Callback;

		virtual int activate( action_activation_ctx_t* ctx ) override
		{
			for ( auto& Pair : c_FbSigs )
			{
				auto Result = Core::FindPattern( Pair.second.c_str( ) );

				if ( Result == BADADDR )
				{
					msg( "[!] Failed to find fb2014 firsttypeinfo!\n" );
					return 0;
				}

				msg( "[!] Found firttypeinfo at 0x%p!\n", Result );
				Frostbite::s_pFirstTypeInfo = Result;

				m_Callback( Pair.first, Result );

				return 0;
			}

			

			return 0;
		}


		virtual action_state_t update( action_update_ctx_t* ctx ) override
		{
			return AST_ENABLE;
		}


	};
}