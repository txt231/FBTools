#pragma once

#include "../../Util/MemoryPointer.h"

#include "../../Sdk/Fb2/FieldInfo.h"

#include "../IFbField.h"

namespace Frostbite
{
	namespace Fb2
	{
		class Fb2FieldType
			: public IFbField
		{
		public:
			Fb2FieldType( ea_t typeInfo, ea_t typeData = BADADDR )
			{
				ea_t TypeData = typeData;


				auto IsValidInfo = ( typeInfo != 0 &&
									 typeInfo != BADADDR );
				auto IsValidData = ( typeData != 0 &&
									 typeData != BADADDR );

				if ( !IsValidData && IsValidInfo )
				{
					Util::MemoryPointer<fb2::TypeInfo> TypeRef( typeInfo );

					auto pTypeInfo = TypeRef.Get( );

					if ( !pTypeInfo )
						return;

					TypeData = pTypeInfo->m_pData.m_Ptr;
				}

				if ( TypeData == 0 ||
					 TypeData == BADADDR )
					return;

				Util::MemoryPointer<fb2::FieldInfo::FieldInfoData> TypeDataRef( TypeData );

				fb2::FieldInfo::FieldInfoData* pData = TypeDataRef;

				if ( !pData )
					return;

				m_TypeData = TypeData;
				m_TypeInfo = typeInfo;
			}

			ea_t m_TypeInfo = BADADDR;
			ea_t m_TypeData = BADADDR;


			virtual bool IsValid( ) override
			{
				return m_TypeData != BADADDR;
			}

			virtual ea_t GetTypeInfo( ) override
			{
				return m_TypeInfo;
			}

			virtual ea_t GetTypeData( ) override
			{
				return m_TypeData;
			}


			virtual bool GetName( std::string& outString ) override
			{
				if ( !this->IsValid( ) )
					return false;

				Util::MemoryPointer<fb2::FieldInfo::FieldInfoData> TypeDataRef( m_TypeData );

				fb2::FieldInfo::FieldInfoData* pData = TypeDataRef;

				if ( !pData )
					return false;

				std::string NameString = pData->m_pName.GetString( );

				if ( NameString.size( ) == 0 )
				{
					char Buffer[256] = { 0 };

					sprintf_s( Buffer, "type_0x%p", m_TypeData );

					outString += std::string( Buffer );
				}
				else
				{
					outString += NameString;
				}

				return true;
			}


			virtual bool GetType( fb::BasicTypesEnum& type )override
			{
				fb::MemberInfoFlags Flags;

				if ( !GetFlags( Flags ) )
					return false;

				type = Flags.GetTypeCode( );

				return true;
			}

			virtual bool GetTypeCategory( fb::TypeCategoryEnum& typeCategory )override
			{
				fb::MemberInfoFlags Flags;

				if ( !GetFlags( Flags ) )
					return false;

				typeCategory = Flags.GetTypeCategory( );

				return true;
			}

			virtual bool GetMemberType( fb::MemberTypeEnum& memberType )override
			{
				fb::MemberInfoFlags Flags;

				if ( !GetFlags( Flags ) )
					return false;

				memberType = Flags.GetMemberType( );

				return true;
			}

			bool GetFlags( fb::MemberInfoFlags& flags ) 
			{
				if ( !this->IsValid( ) )
					return false;

				Util::MemoryPointer<fb2::FieldInfo::FieldInfoData> TypeDataRef( m_TypeData );

				fb2::FieldInfo::FieldInfoData* pData = TypeDataRef;

				if ( !pData )
					return false;

				flags = pData->m_Flags;

				return true;
			}

			virtual ea_t GetFieldType( ) override
			{
				if ( !this->IsValid( ) )
					return BADADDR;

				Util::MemoryPointer<fb2::FieldInfo::FieldInfoData> TypeDataRef( m_TypeData );

				fb2::FieldInfo::FieldInfoData* pData = TypeDataRef;

				if ( !pData )
					return BADADDR;

				if ( pData->m_pFieldType.m_Ptr == 0 )
					return BADADDR;

				return pData->m_pFieldType.m_Ptr;
			}

			virtual int32_t GetOffset( ) override
			{
				if ( !this->IsValid( ) )
					return -1;

				Util::MemoryPointer<fb2::FieldInfo::FieldInfoData> TypeDataRef( m_TypeData );

				fb2::FieldInfo::FieldInfoData* pData = TypeDataRef;

				if ( !pData )
					return -1;

				return pData->m_FieldOffset;
			}
		};
	}
}