#pragma once

#include <unordered_set>

#include <name.hpp>

#include "../ActionBase.h"

#include "../Core/Pattern.h"

#include "../Frostbite/FrostbiteUtil.h"

#include "../Util/MemoryPointer.h"

namespace Actions
{

	class DumpDefinitions
		: public Action::ActionBase
	{
	public:
		DumpDefinitions( Frostbite::FBVersion type )
			: ActionBase( "FBTools:DumpDefinitions",
						  "Dumps TypeInfo Definitions",
						  "Prints TypeInfo Definitions into console" )
		{
		}

		virtual int activate( action_activation_ctx_t* ctx ) override
		{
			if ( !Frostbite::ReadTypeInfos( ) )
			{
				msg( "[!] Found no typeinfo!?!\n" );
				return 0;
			}

			for ( auto* pType : Frostbite::s_FbTypes )
			{
				std::string TypeName;

				if ( !pType->GetName( TypeName ) )
					continue;

				fb::BasicTypesEnum Type;

				if ( !pType->GetType( Type ) )
					continue;

				switch ( Type )
				{
				case fb::BTE_Class:
					msg( "class %s\n", TypeName.c_str( ) );
					break;

				case fb::BTE_ValueType:
					msg( "struct %s\n", TypeName.c_str( ) );
					break;

				case fb::BTE_Enum:
					msg( "enum %s\n", TypeName.c_str( ) );
					break;


				case fb::BTE_Action:
					msg( "Action %s\n", TypeName.c_str( ) );
					break;
				}
			}

			return 0;
		}

		virtual action_state_t update( action_update_ctx_t* ctx ) override
		{
			return Frostbite::s_FbTypes.size( ) == 0 ? AST_DISABLE : AST_ENABLE;
		}
	};
}