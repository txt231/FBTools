#pragma once

#include "../../Util/MemoryPointer.h"

#include "../../Sdk/fb2017/TypeInfo.h"


#include "../IFbType.h"


namespace Frostbite
{
	namespace Fb2017
	{
		class Fb2017Type
			: public IFbType
		{
		public:
			Fb2017Type( ea_t typeInfo, ea_t typeData = BADADDR )
			{
				ea_t TypeData = typeData;


				auto IsValidInfo = ( typeInfo != 0 &&
								 typeInfo != BADADDR );
				auto IsValidData = ( typeData != 0 &&
									 typeData != BADADDR );
				
				if ( !IsValidData && IsValidInfo )
				{
					Util::MemoryPointer<fb2017::TypeInfo> TypeRef( typeInfo );

					auto pTypeInfo = TypeRef.Get( );

					if ( !pTypeInfo )
						return;

					TypeData = pTypeInfo->m_pData.m_Ptr;
				}

				if ( TypeData == 0 ||
					 TypeData == BADADDR )
					return;

				Util::MemoryPointer<fb2017::TypeInfo::TypeInfoData> TypeDataRef( TypeData );

				fb2017::TypeInfo::TypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return;

				if ( pData->m_Flags.GetTypeCategory( ) != fb::TypeCategoryEnum::TCE_PrimitiveType )
				{
					auto TypeCode = pData->m_Flags.GetTypeCode( );

					if ( TypeCode != fb::BasicTypesEnum::BTE_Class &&
						 TypeCode != fb::BasicTypesEnum::BTE_ValueType &&
						 TypeCode != fb::BasicTypesEnum::BTE_Enum &&
						 TypeCode != fb::BasicTypesEnum::BTE_Action ) // Action support
						return;

				}
			

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

				Util::MemoryPointer<fb2017::TypeInfo::TypeInfoData> TypeDataRef( m_TypeData );

				fb2017::TypeInfo::TypeInfoData* pData = TypeDataRef;

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
				fb2017::MemberInfoFlags Flags;

				if ( !GetFlags( Flags ) )
					return false;

				type = Flags.GetTypeCode( );

				return true;
			}

			virtual bool GetTypeCategory( fb::TypeCategoryEnum& typeCategory )override
			{
				fb2017::MemberInfoFlags Flags;

				if ( !GetFlags( Flags ) )
					return false;

				typeCategory = Flags.GetTypeCategory( );

				return true;
			}

			virtual bool GetMemberType( fb::MemberTypeEnum& memberType )override
			{
				fb2017::MemberInfoFlags Flags;

				if ( !GetFlags( Flags ) )
					return false;

				memberType = Flags.GetMemberType( );

				return true;
			}

			bool GetFlags( fb2017::MemberInfoFlags& flags ) 
			{
				if ( !this->IsValid( ) )
					return false;

				Util::MemoryPointer<fb2017::TypeInfo::TypeInfoData> TypeDataRef( m_TypeData );

				fb2017::TypeInfo::TypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return false;

				flags = pData->m_Flags;

				return true;
			}

			 
			virtual ea_t GetModuleAddress( ) override
			{
				if ( !this->IsValid( ) )
					return BADADDR;

				Util::MemoryPointer<fb2017::TypeInfo::TypeInfoData> TypeDataRef( m_TypeData );

				fb2017::TypeInfo::TypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return BADADDR;


				if ( pData->m_pModule.m_Ptr == 0 )
					return BADADDR;

				return pData->m_pModule.m_Ptr;
			}

			virtual bool GetModuleName( std::string& moduleName ) override
			{
				if ( !this->IsValid( ) )
					return false;

				Util::MemoryPointer<fb2017::TypeInfo::TypeInfoData> TypeDataRef( m_TypeData );

				fb2017::TypeInfo::TypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return false;

				auto pModule = pData->m_pModule.Get( );

				if ( !pModule )
					return false;

				std::string NameString = pData->m_pName.GetString( );

				if ( NameString.size( ) == 0 )
					return false;

				moduleName += NameString;

				return true;

			}


			virtual ea_t GetDefaultInstance( ) override
			{
				return BADADDR;
			}

			virtual int32_t GetSize( ) override
			{
				if ( !this->IsValid( ) )
					return -1;

				Util::MemoryPointer<fb2017::TypeInfo::TypeInfoData> TypeDataRef( m_TypeData );

				fb2017::TypeInfo::TypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return -1;

				return pData->m_Size;
			}

			virtual int32_t GetAlignment( ) override
			{
				if ( !this->IsValid( ) )
					return 1;

				Util::MemoryPointer<fb2017::TypeInfo::TypeInfoData> TypeDataRef( m_TypeData );

				fb2017::TypeInfo::TypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return 1;

				return pData->m_Alignment;
			}


			virtual int32_t GetFieldCount( ) override
			{
				if ( !this->IsValid( ) )
					return- 1;

				Util::MemoryPointer<fb2017::TypeInfo::TypeInfoData> TypeDataRef( m_TypeData );

				fb2017::TypeInfo::TypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return -1;

				return pData->m_FieldCount;
			}



			static Fb2017Type* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;

				return new Fb2017Type( typeInfo );
			}

			static Fb2017Type* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2017Type( BADADDR, typeInfoData );
			}
		};
	}
}