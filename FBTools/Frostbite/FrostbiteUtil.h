#pragma once

#include <ida.hpp>

#include <vector>


#include <bytes.hpp>


#include <struct.hpp>

#include "FBVersion.h"

#include "TypeFactory.h"

#include "../Util/Common.h"

namespace Frostbite
{

	static ea_t s_pFirstTypeInfo = BADADDR;
	static FBVersion s_FbVersion = FBT_Invalid;

	static std::vector<IFbType*> s_FbTypes;

	

	static bool ReadTypeInfosByFirstType( )
	{
		if ( s_pFirstTypeInfo == BADADDR )
			return false;

		//TODO: Delete

		for ( auto* pInstance : s_FbTypes )
			delete pInstance;
		s_FbTypes.clear( );

		ea_t Data = Util::ReadEA( Frostbite::s_pFirstTypeInfo );

		while ( Data != 0 && 
				Data != BADADDR )
		{
			auto* pType = TypeFactory::CreateType( s_FbVersion, Data );

			if ( pType != nullptr )
			{
				if ( pType->IsValid( ) )
				{
					s_FbTypes.push_back( pType );
				}
				else
				{
					delete pType;
				}
			}

			Data = TypeFactory::GetNextType( s_FbVersion, Data );
		}

		return s_FbTypes.size( ) > 0;
	}

	static bool ReadTypeInfosByRefrences( )
	{
		//TODO: Find data only somehow...

		// Can be useful for xbox, ps3 and undumped pc binaries(server files for example)


		return false;
	}

	static bool ReadTypeInfos( )
	{
		if ( s_FbTypes.size( ) > 0 )
			return true;

		if ( s_pFirstTypeInfo != BADADDR )
			return ReadTypeInfosByFirstType( );
		else
			return ReadTypeInfosByRefrences( );
	}


	static IFbType* GetTypeFromTypeInfoEA( ea_t typeInfo )
	{
		for ( auto* pType : s_FbTypes )
		{
			if ( pType->GetTypeInfo( ) != typeInfo )
				continue;

			return pType;
		}

		return nullptr;
	}

	static IFbType* GetTypeFromTypeDataEA( ea_t typeData )
	{
		for ( auto* pType : s_FbTypes )
		{
			if ( pType->GetTypeData( ) != typeData )
				continue;

			return pType;
		}

		return nullptr;
	}

	//TODO: REDO
	/*
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
	*/
}