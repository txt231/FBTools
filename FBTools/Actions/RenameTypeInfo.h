#pragma once

#include <unordered_set>

#include <name.hpp>

#include "../ActionBase.h"

#include "../Core/Pattern.h"

#include "../Frostbite/FrostbiteUtil.h"


#include "../Util/MemoryPointer.h"

#include "../Sdk/2014.4/TypeInfo.h"
#include "../Sdk/2014.4/ValueTypeInfo.h"

namespace Actions
{

	class RenameTypeInfo
		: public Action::ActionBase
	{
	public:
		RenameTypeInfo( Frostbite::FBType type)
			: ActionBase( "FBTools:RenameTypeInfo",
						  "Rename TypeInfo",
						  "Renames TypeInfo and TypeInfoData" )
		{ }

		virtual int activate( action_activation_ctx_t* ctx ) override
		{
			std::unordered_set<ea_t> ModuleFilter;


			std::vector<std::pair<ea_t, ea_t>> Types;

			if ( !Frostbite::FindTypeInfos( Types ) )
			{
				msg( "[!] Found no typeinfo!?!\n" );
				return 0;
			}

			for ( auto& TypePairs : Types )
			{
				Util::MemoryPointer<fb::TypeInfo> TypeRef( TypePairs.first );


				Util::MemoryPointer<fb::TypeInfo::TypeInfoData> TypeDataRef( TypePairs.second );

				fb::TypeInfo::TypeInfoData* pData = TypeDataRef;

				if ( !pData )
					continue;

				if ( pData->m_pModule.m_Ptr && ModuleFilter.find( pData->m_pModule.m_Ptr ) != ModuleFilter.end( ) )
				{
					fb::ModuleInfo* pModule = pData->m_pModule;

					if ( pModule )
					{
						std::string NameString = pModule->m_pModuleName.GetString( );

						if ( NameString.size( ) > 0 )
						{
							set_name( pData->m_pModule.m_Ptr, ( "?c_ModuleInfo@" + NameString + "_Module@@2VModuleInfo@fb@@A" ).c_str( ), SN_NOCHECK );

							ModuleFilter.insert( pData->m_pModule.m_Ptr );
						}
					}
				}

				auto Name = pData->m_pName;

				std::string NameString = Name.GetString( );

				if ( NameString.size( ) == 0 )
					continue;


				msg( "[+] Found typeinfo %s\n", NameString.c_str( ) );


				auto Type = pData->m_Flags.GetTypeCode( );

				switch ( Type )
				{
				case fb::BTE_Class:
				{
					set_name( TypeDataRef.m_Ptr, ( "?c_TypeInfoData@" + NameString + "@fb@@1UClassInfoData@ClassInfo@2@B" ).c_str( ), SN_NOCHECK );
					set_name( TypeRef.m_Ptr, ( "?c_TypeInfo@" + NameString + "@fb@@2VClassInfo@2@A" ).c_str( ), SN_NOCHECK );
					break;
				}
				case fb::BTE_ValueType:
				{
					set_name( TypeDataRef.m_Ptr, ( "?c_TypeInfoData@" + NameString + "@fb@@1UValueTypeInfoData@ValueTypeInfo@2@B" ).c_str( ), SN_NOCHECK );
					set_name( TypeRef.m_Ptr, ( "?c_TypeInfo@" + NameString + "@fb@@2VValueTypeInfo@2@A" ).c_str( ), SN_NOCHECK );

					Util::MemoryPointer<fb::ValueTypeInfo::ValueTypeInfoData> ValueTypeDataRef( TypeDataRef.m_Ptr );

					fb::ValueTypeInfo::ValueTypeInfoData* pValueData = ValueTypeDataRef.Get( );

					if ( pValueData && pValueData->m_pDefaultValue )
					{
						set_name( pValueData->m_pDefaultValue, ( "_default_" + NameString ).c_str( ), SN_NOCHECK );
					}

					break;
				}

				case fb::BTE_Enum:
				{
					set_name( TypeDataRef.m_Ptr, ( "?c_TypeInfoData@" + NameString + "@fb@@1UEnumTypeInfoData@EnumTypeInfo@2@B" ).c_str( ), SN_NOCHECK );
					set_name( TypeRef.m_Ptr, ( "?c_TypeInfo@" + NameString + "@fb@@2VEnumTypeInfo@2@B" ).c_str( ), SN_NOCHECK );
					break;
				}

				default:
				{
					set_name( TypeDataRef.m_Ptr, ( "TypeInfoData_" + NameString ).c_str( ), SN_NOCHECK );
					set_name( TypeRef.m_Ptr, ( "TypeInfo_" + NameString ).c_str( ), SN_NOCHECK );
					break;
				}
				}

			}



			return 0;
		}


		virtual action_state_t update( action_update_ctx_t* ctx ) override
		{
			return Frostbite::s_pFirstTypeInfo == BADADDR ? AST_DISABLE : AST_ENABLE;
		}


	};
}