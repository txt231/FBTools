#pragma once

#include <unordered_set>

#include <name.hpp>

#include "../ActionBase.h"

#include "../Core/Pattern.h"

#include "../Frostbite/FrostbiteUtil.h"


#include "../Util/MemoryPointer.h"

namespace Actions
{

	class RenameTypeInfo
		: public Action::ActionBase
	{
	public:
		RenameTypeInfo( Frostbite::FBVersion type )
			: ActionBase( "FBTools:RenameTypeInfo",
						  "Rename TypeInfo",
						  "Renames TypeInfo and TypeInfoData" )
		{
		}

		virtual int activate( action_activation_ctx_t* ctx ) override
		{
			std::unordered_set<ea_t> ModuleFilter;


			if ( !Frostbite::ReadTypeInfos( ) )
			{
				msg( "[!] Found no typeinfo!?!\n" );
				return 0;
			}

			for ( auto* pType : Frostbite::s_FbTypes )
			{
				auto ModuleAddress = pType->GetModuleAddress( );
				if ( ModuleAddress != BADADDR &&
					 ModuleFilter.find( ModuleAddress ) != ModuleFilter.end( ) )
				{
					std::string ModuleName;
					if ( pType->GetModuleName( ModuleName ) )
					{
						set_name( ModuleAddress, ( "?c_ModuleInfo@" + ModuleName + "_Module@@2VModuleInfo@fb@@A" ).c_str( ), SN_NOCHECK );

						ModuleFilter.insert( ModuleAddress );
					}
				}

				std::string TypeName;

				if ( !pType->GetName( TypeName ) )
					continue;


				msg( "[+] Found typeinfo %s\n", TypeName.c_str( ) );

				fb::BasicTypesEnum Type;

				if ( !pType->GetType( Type ) )
					continue;

				std::string TypeDataName = "TypeInfoData_" + TypeName;
				std::string TypeInfoName = "TypeInfo_" + TypeName;

				switch ( Type )
				{
				case fb::BTE_Class:
					TypeDataName = "?c_TypeInfoData@" + TypeName + "@fb@@1UClassInfoData@ClassInfo@2@B";
					TypeInfoName = "?c_TypeInfo@" + TypeName + "@fb@@2VClassInfo@2@A";
					break;

				case fb::BTE_ValueType:
					TypeDataName = "?c_TypeInfoData@" + TypeName + "@fb@@1UValueTypeInfoData@ValueTypeInfo@2@B";
					TypeInfoName = "?c_TypeInfo@" + TypeName + "@fb@@2VValueTypeInfo@2@A";

					if ( pType->GetDefaultInstance( ) != BADADDR )
					{
						set_name( pType->GetDefaultInstance( ), ( "_default_" + TypeName ).c_str( ), SN_NOCHECK );
					}

					break;

				case fb::BTE_Enum:
					TypeDataName = "?c_TypeInfoData@" + TypeName + "@fb@@1UEnumTypeInfoData@EnumTypeInfo@2@B";
					TypeInfoName = "?c_TypeInfo@" + TypeName + "@fb@@2VEnumTypeInfo@2@B";
					break;


				case fb::BTE_Action:
					TypeDataName = "?c_TypeInfoData@" + TypeName + "@fb@@1UActionTypeInfoData@ActionTypeInfo@2@B";
					TypeInfoName = "?c_TypeInfo@" + TypeName + "@fb@@2VActionTypeInfo@2@A";
					break;
				}

				if ( pType->GetTypeData( ) != BADADDR )
					set_name( pType->GetTypeData( ), TypeDataName.c_str( ), SN_NOCHECK );

				if ( pType->GetTypeInfo( ) != BADADDR )
					set_name( pType->GetTypeInfo( ), TypeInfoName.c_str( ), SN_NOCHECK );
			}



			return 0;
		}


		virtual action_state_t update( action_update_ctx_t* ctx ) override
		{
			return Frostbite::s_FbTypes.size( ) == 0 ? AST_DISABLE : AST_ENABLE;
		}


	};
}