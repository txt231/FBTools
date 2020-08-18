#pragma once

#include <unordered_set>

#include <name.hpp>
#include <struct.hpp>

#include "../ActionBase.h"

#include "../Core/Pattern.h"

#include "../Frostbite/FrostbiteUtil.h"


#include "../Util/MemoryPointer.h"

#include "../Sdk/fb2014.4/TypeInfo.h"
#include "../Sdk/fb2014.4/ValueTypeInfo.h"

namespace Actions
{

	class CreateTypes
		: public Action::ActionBase
	{
	public:
		CreateTypes( Frostbite::FBVersion type )
			: ActionBase( "FBTools:CreateTypes",
						  "Create Local Types",
						  "Creates local types from valueinfos and enums" )
		{ }

		virtual int activate( action_activation_ctx_t* ctx ) override
		{
			if ( Frostbite::s_pFirstTypeInfo == BADADDR )
				return 0;



			std::vector<std::pair<ea_t, ea_t>> Types;

			if ( !Frostbite::FindTypeInfos( Types ) )
				return 0;

			for ( auto& TypePairs : Types )
			{
				
			}



			return 0;
		}


		virtual action_state_t update( action_update_ctx_t* ctx ) override
		{
			return Frostbite::s_pFirstTypeInfo == BADADDR ? AST_DISABLE : AST_ENABLE;
		}


	};
}