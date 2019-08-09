#pragma once

#include "../ActionBase.h"

#include "../Core/Pattern.h"

#include "../Frostbite/FrostbiteUtil.h"

namespace Actions
{

	class FindTypeInfoAction
		: public Action::ActionBase
	{
	public:
		FindTypeInfoAction( )
			: ActionBase( "FBTools:FindTypeInfo",
						  "Find FirstTypeInfo",
						  "Finds FirstTypeInfo so you can use other actions..." )
		{
		}

		virtual int activate( action_activation_ctx_t* ctx ) override
		{
			auto Result = Core::FindPattern( "48 C7 43 ?? ?? ?? ?? ??"
											 "48 8B 05 [ ?? ?? ?? ?? ]"
											 "48 89 43 08" );

			if ( Result == BADADDR )
			{
				msg( "[!] Failed to find fb2014 firsttypeinfo!\n" );
				return 0;
			}

			msg( "[!] Found firttypeinfo at 0x%p!\n", Result );
			Frostbite::s_pFirstTypeInfo = Result;

			return 0;
		}


		virtual action_state_t update( action_update_ctx_t* ctx ) override
		{
			return AST_ENABLE;
		}


	};
}