#pragma once

#include <unordered_set>

#include <name.hpp>
#include <struct.hpp>

#include "../ActionBase.h"

#include "../Core/Pattern.h"

#include "../Frostbite/FrostbiteUtil.h"

#include "../Util/MemoryPointer.h"

#include "../Frostbite/IFbField.h"
#include "../Frostbite/IFbType.h"

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
		{
		}

		virtual int activate( action_activation_ctx_t* ctx ) override
		{
			if ( Frostbite::s_pFirstTypeInfo == BADADDR )
				return 0;


			if ( !Frostbite::ReadTypeInfos( ) )
				return 0;




			return 0;
		}


		virtual action_state_t update( action_update_ctx_t* ctx ) override
		{
			return Frostbite::s_pFirstTypeInfo == BADADDR ? AST_DISABLE : AST_ENABLE;
		}


		static void CreateStructFromTypeInfo( Frostbite::IFbType* pType );


		static void AddFieldToStruct( struc_t* pStruct, Frostbite::IFbField* pField )
		{
			if ( !pStruct )
				return;

			if ( !pField )
				return;

			auto FieldTypeData = pField->GetFieldType( );

			if ( FieldTypeData == BADADDR )
			{
				//Create type based on flags

				return;
			}

			auto* pFieldType = Frostbite::GetTypeFromTypeInfoEA( FieldTypeData );

			if ( !pFieldType )
				return;

			fb::BasicTypesEnum Type;

			if ( !pField->GetType( Type ) )
				return;

			std::string FieldName;

			if ( !pField->GetName( FieldName ) )
				return;

			auto fAddBasicStructMember = [&]( flags_t flags )
			{
				add_struc_member( pStruct, FieldName.c_str( ), pField->GetOffset( ), flags, nullptr, pFieldType->GetSize( ) );
			};

			switch ( Type )
			{
			case fb::BTE_ValueType:
			{
				std::string FieldTypeName;

				if ( !pFieldType->GetName( FieldTypeName ) )
				{
					msg( "[!!] Severe error with structure stuff\n" );
					return;
				}

				auto StructId = get_struc_id( FieldTypeName.c_str( ) );

				if ( StructId == BADADDR )
				{
					CreateStructFromTypeInfo( pFieldType );
					StructId = get_struc_id( FieldTypeName.c_str( ) );
				}


				if ( StructId == BADADDR )
				{
					msg( "[!!] Severe error with structure stuff\n" );
					return;
				}


				opinfo_t OpInfo;
				OpInfo.tid = StructId;

				add_struc_member( pStruct, FieldName.c_str( ), pField->GetOffset( ), stru_flag(), &OpInfo, pFieldType->GetSize( ) );
				break;
			}
			case fb::BTE_Class:
			{



				break;
			}
			case fb::BTE_Array:


			case fb::BTE_FixedArray:
				//array types is going to be strange lol
				break;

			case fb::BTE_String: //char[32]
				break;
			case fb::BTE_CString: //const char*
			{
				opinfo_t OpInfo;
				OpInfo.strtype = STRTYPE_C;


				add_struc_member( pStruct, FieldName.c_str( ), pField->GetOffset( ), Util::PtrFlag( ), &OpInfo, pFieldType->GetSize( ) );
				break;
			}

			case fb::BTE_Enum:
			{
				std::string FieldTypeName;

				if ( !pFieldType->GetName( FieldTypeName ) )
				{
					msg( "[!!] Severe error with structure stuff\n" );
					return;
				}

				auto EnumId = get_struc_id( FieldTypeName.c_str( ) );

				if ( EnumId == BADADDR )
				{
					CreateStructFromTypeInfo( pFieldType );
					EnumId = get_struc_id( FieldTypeName.c_str( ) );
				}


				if ( EnumId == BADADDR )
				{
					msg( "[!!] Severe error with structure stuff\n" );
					return;
				}

				opinfo_t OpInfo;
				OpInfo.ec.tid = EnumId;

				add_struc_member( pStruct, FieldName.c_str( ), pField->GetOffset( ), get_flags_by_size( pFieldType->GetSize( ) ), &OpInfo, pFieldType->GetSize( ) );
				break;
			}
				

			case fb::BTE_FileRef:
				//Fileref is a 64 bit id, that is turned into a ptr at runtime
				fAddBasicStructMember( Util::PtrFlag( ) );
				break;
			case fb::BTE_Boolean:
			case fb::BTE_Int8:
			case fb::BTE_Uint8:

				fAddBasicStructMember( byte_flag( ) );
				break;
			case fb::BTE_Int16:
			case fb::BTE_Uint16:
			case fb::BTE_Int32:
			case fb::BTE_Uint32:
				fAddBasicStructMember( dword_flag( ) );
				break;
			case fb::BTE_Int64:
			case fb::BTE_Uint64:
				fAddBasicStructMember( qword_flag( ) );
				break;
			case fb::BTE_Float32:
				fAddBasicStructMember( float_flag( ) );
				break;
			case fb::BTE_Float64:
				fAddBasicStructMember( double_flag( ) );
				break;
				//case fb::BTE_Guid:
				//case fb::BTE_SHA1:


			case fb::BTE_ResourceRef:
				fAddBasicStructMember( Util::PtrFlag( ) );
				break;
			default:
				msg( "Type not supported!\n" );
				return;
			}


		};


		static void CreateStructFromTypeInfo( Frostbite::IFbType* pType )
		{


			fb::BasicTypesEnum Type;

			if ( !pType->GetType( Type ) )
				return;


			//TODO: Add classes with some shitty inherritance ;)
			if ( Type != fb::BTE_ValueType ||
				 Type != fb::BTE_Enum )
				return;

			if ( pType->GetFieldCount( ) == 0 )
				return;


			std::string NameString;

			if ( !pType->GetName( NameString ) )
				return;

			switch ( Type )
			{
			case fb::BTE_ValueType:

				auto StructId = get_struc_id( NameString.c_str( ) );

				if ( StructId == BADADDR )
					return;


				auto StructId = add_struc( BADADDR, NameString.c_str( ) );

				auto pStruct = get_struc( StructId );

				if ( !pStruct )
					return;

				std::vector<Frostbite::IFbField*> Fields;

				if ( !pType->GetFields( Fields ) )
					return;

				for ( auto* pField : Fields )
				{
					AddFieldToStruct( pStruct, pField );
				}

				//case fb::BTE_Enum:


			}
		}
	};
}