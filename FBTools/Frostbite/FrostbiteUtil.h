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


}