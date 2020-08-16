#pragma once

#include "FBVersion.h"

#include "IFbType.h"


#include "Fb2/Fb2Type.h"
#include "Fb2/Fb2ClassType.h"
#include "Fb2/Fb2ValueType.h"
#include "Fb2/Fb2EnumType.h"

#include "fb2014.4/Fb2014_4Type.h"
#include "fb2014.4/Fb2014_4ClassType.h"
#include "fb2014.4/Fb2014_4ValueType.h"
#include "fb2014.4/Fb2014_4EnumType.h"

namespace Frostbite
{
	class TypeFactory
	{
	public:
		static IFbType* Fb2_CreateType( ea_t typeInfo )
		{
			Fb2::Fb2Type* pTempType = Fb2::Fb2Type::CreateFromTypeInfo( typeInfo );

			if ( !pTempType )
				return nullptr;

			if ( !pTempType->IsValid( ) )
			{
				delete pTempType;
				return nullptr;
			}

			fb::MemberInfoFlags Flags;

			if ( !pTempType->GetFlags( Flags ) )
			{
				delete pTempType;
				return nullptr;
			}

			delete pTempType;


			switch ( Flags.GetTypeCode( ) )
			{
			case fb::BasicTypesEnum::BTE_Class:
				return Fb2::Fb2ClassType::CreateFromTypeInfo( typeInfo );

			case fb::BasicTypesEnum::BTE_ValueType:
				return Fb2::Fb2ValueType::CreateFromTypeInfo( typeInfo );

			case fb::BasicTypesEnum::BTE_Enum:
				return Fb2::Fb2EnumType::CreateFromTypeInfo( typeInfo );
			}

			return nullptr;
		}


		static IFbType* Fb2014_CreateType( ea_t typeInfo )
		{
			Fb2014_4::Fb2014_4Type* pTempType = Fb2014_4::Fb2014_4Type::CreateFromTypeInfo( typeInfo );

			if ( !pTempType )
				return nullptr;

			if ( !pTempType->IsValid( ) )
			{
				delete pTempType;
				return nullptr;
			}

			fb::MemberInfoFlags Flags;

			if ( !pTempType->GetFlags( Flags ) )
			{
				delete pTempType;
				return nullptr;
			}

			delete pTempType;


			switch ( Flags.GetTypeCode( ) )
			{
			case fb::BasicTypesEnum::BTE_Class:
				return Fb2014_4::Fb2014_4ClassType::CreateFromTypeInfo( typeInfo );

			case fb::BasicTypesEnum::BTE_ValueType:
				return Fb2014_4::Fb2014_4ValueType::CreateFromTypeInfo( typeInfo );

			case fb::BasicTypesEnum::BTE_Enum:
				return Fb2014_4::Fb2014_4EnumType::CreateFromTypeInfo( typeInfo );
			}

			return nullptr;
		}


		static IFbType* CreateType( FBVersion version, ea_t typeInfo )
		{
			switch ( version )
			{
			case FBVersion::FBT_Fb2:
				return Fb2_CreateType( typeInfo );
			case FBVersion::FBT_Fb2014:
				return Fb2014_CreateType( typeInfo );
			}

			msg( "[!] Fb version %i not implimented! Add support in %s", version, __FILE__ );
			return nullptr;
		}

		static ea_t Fb2_GetNextType( ea_t typeInfo )
		{
			Util::MemoryPointer<fb2::TypeInfo> TypeRef( typeInfo );

			auto pTypeInfo = TypeRef.Get( );

			if ( !pTypeInfo )
				return BADADDR;

			if ( pTypeInfo->m_pNext.m_Ptr == 0 )
				return BADADDR;

			return pTypeInfo->m_pNext.m_Ptr;
		}

		static ea_t Fb2014_GetNextType( ea_t typeInfo )
		{
			Util::MemoryPointer<fb2014_4::TypeInfo> TypeRef( typeInfo );

			auto pTypeInfo = TypeRef.Get( );

			if ( !pTypeInfo )
				return BADADDR;

			if ( pTypeInfo->m_pNext.m_Ptr == 0 )
				return BADADDR;

			return pTypeInfo->m_pNext.m_Ptr;
		}

		static ea_t GetNextType( FBVersion version, ea_t typeInfo )
		{
			if ( typeInfo == 0 ||
				 typeInfo == BADADDR )
				return BADADDR;

			switch ( version )
			{
			case FBVersion::FBT_Fb2:
				return Fb2_GetNextType( typeInfo );
			case FBVersion::FBT_Fb2014:
				return Fb2014_GetNextType( typeInfo );
			}

			msg( "[!] Fb version %i not implimented! Add support in %s", version, __FILE__ );
			return BADADDR;
		}
	};
}