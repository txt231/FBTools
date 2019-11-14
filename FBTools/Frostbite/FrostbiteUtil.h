#pragma once

#include <ida.hpp>

#include <vector>


#include <bytes.hpp>


#include <struct.hpp>

#include "FBType.h"

#include "../Sdk/2014.4/FieldInfo.h"

#include "../Sdk/2014.4/ValueTypeInfo.h"

namespace Frostbite
{

	static ea_t s_pFirstTypeInfo = BADADDR;



	static bool FindTypeInfosByFirstType( std::vector<std::pair<ea_t, ea_t>>& outTypeAddresses )
	{
		if ( s_pFirstTypeInfo == BADADDR )
			return false;

		ea_t Data = (ea_t )get_qword( Frostbite::s_pFirstTypeInfo );

		Util::MemoryPointer<fb::TypeInfo> TypeRef( Data );



		for ( ; TypeRef.m_Ptr, TypeRef = TypeRef.Get( )->m_pNext; )
		{
			fb::TypeInfo* pType = TypeRef.Get( );

			if ( !pType )
				break;

			outTypeAddresses.push_back( { TypeRef.m_Ptr ,pType->m_pData.m_Ptr } );
		}

		return outTypeAddresses.size( ) > 0;
	}

	static bool FindTypeInfosByRefrences( std::vector<std::pair<ea_t, ea_t>>& outTypeAddresses )
	{
		//TODO: FInd it somehow...
		return false;
	}

	static bool FindTypeInfos( std::vector<std::pair<ea_t, ea_t>>& outTypeAddresses )
	{
		if ( s_pFirstTypeInfo != BADADDR )
			return FindTypeInfosByFirstType( outTypeAddresses );
		else
			return FindTypeInfosByRefrences( outTypeAddresses );
	}



	static void CreateStructFromTypeInfo( ea_t typeData );
	

	static void AddFieldInfoToStruct( struc_t* pStruct, fb::FieldInfo::FieldInfoData* pData )
	{
		if ( !pStruct )
			return;

		if ( !pData )
			return;

		auto pFieldType = pData->m_pFieldType.Get( );

		if ( !pFieldType )
		{
			//Create type based on flags

			return;
		}


		Util::MemoryPointer<fb::TypeInfo::TypeInfoData> FieldTypeDataRef( pFieldType->m_pData.m_Ptr );

		auto pFieldTypeData = FieldTypeDataRef.Get( );

		if ( !pFieldTypeData )
			return;

		auto Type = pFieldTypeData->m_Flags.GetTypeCode( );

		auto fAddBasicStructMember = [&]( flags_t flags )
		{
			add_struc_member( pStruct, pData->m_pName.GetString( ).c_str( ), pData->m_FieldOffset, flags, nullptr, pFieldTypeData->m_Size );

		};

		switch ( Type )
		{
		case fb::BTE_ValueType:
		case fb::BTE_Class:
		{
			auto StructId = get_struc_id( pFieldTypeData->m_pName.GetString( ).c_str( ) );

			if ( StructId == BADADDR )
			{
				CreateStructFromTypeInfo( FieldTypeDataRef.m_Ptr );
				StructId = get_struc_id( pFieldTypeData->m_pName.GetString( ).c_str( ) );
			}


			if ( StructId == BADADDR )
			{
				msg( "[!!] Severe error with structure stuff\n" );
				return;
			}


			opinfo_t OpInfo;


			break;
		}
		case fb::BTE_Array:

			
		case fb::BTE_FixedArray:
			//array types is going to be strange lol
			break;

		case fb::BTE_String: //char*
			break;
		case fb::BTE_CString: //const char*
		{
			opinfo_t OpInfo;
			OpInfo.strtype = STRTYPE_C;

			break;
		}
			
		case fb::BTE_Enum:
			
			break;

		case fb::BTE_FileRef:
			//Fileref is a 64 bit id, that is turned into a ptr at runtime
			fAddBasicStructMember( qword_flag( ) );
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
		case fb::BTE_Float64:
		case fb::BTE_Guid:
		case fb::BTE_SHA1:
		case fb::BTE_ResourceRef:
		default:
			msg( "Type not supported!\n" );
			return;
		}

		
	};


	static void CreateStructFromTypeInfo( ea_t typeData )
	{
		Util::MemoryPointer<fb::TypeInfo::TypeInfoData> TypeDataRef( typeData );

		fb::TypeInfo::TypeInfoData* pData = TypeDataRef;

		if ( !pData )
			return;


		auto Type = pData->m_Flags.GetTypeCode( );



		if ( Type != fb::BTE_ValueType ||
			 Type != fb::BTE_Enum )
			return;

		auto Name = pData->m_pName;

		std::string NameString = Name.GetString( );

		if ( NameString.size( ) == 0 )
			return;

		if ( pData->m_FieldCount == 0 )
			return;


		switch ( Type )
		{
		case fb::BTE_ValueType:
			Util::MemoryPointer<fb::ValueTypeInfo::ValueTypeInfoData> ValueTypeDataRef( TypeDataRef.m_Ptr );


			fb::ValueTypeInfo::ValueTypeInfoData* pValueTypeInfoData = ValueTypeDataRef;

			if ( !pValueTypeInfoData )
				return;

			auto StructId = add_struc( BADADDR, NameString.c_str( ) );

			auto pStruct = get_struc( StructId );

			if ( !pStruct )
				return;

			for ( auto i = 0; i < pData->m_FieldCount; i++ )
			{
				auto pMember = pValueTypeInfoData->m_pFields.At( i );

				if ( !pMember )
					return;

				//add_struc_member( pStruct, );
			}

		//case fb::BTE_Enum:


		}
	}
}