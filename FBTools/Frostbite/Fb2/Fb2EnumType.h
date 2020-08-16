#pragma once

#include "Fb2Type.h"

#include "Fb2EnumFieldType.h"

#include "../../Sdk/Fb2/EnumTypeInfo.h"



namespace Frostbite
{
	namespace Fb2
	{
		class Fb2EnumType
			: public Fb2Type
		{
		public:
			Fb2EnumType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2Type( typeInfo, typeData )
			{
				ReadValues( );
			}

			~Fb2EnumType( )
			{
				for ( auto* pField : m_Values )
					delete pField;

				m_Values.clear( );
			}

			std::vector<Fb2EnumFieldType*> m_Values;


			void ReadValues( )
			{
				if ( !this->IsValid( ) )
					return;

				if ( this->GetFieldCount( ) == 0 )
					return;

				Util::MemoryPointer<fb2::EnumTypeInfo::EnumTypeInfoData> TypeDataRef( m_TypeData );

				fb2::EnumTypeInfo::EnumTypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return;

				if ( pData->m_pFields.m_Ptr == 0 )
					return;

				auto FieldCount = this->GetFieldCount( );
				for ( auto i = 0; i < FieldCount; i++ )
				{
					m_Values.push_back( new Fb2EnumFieldType( pData->m_pFields.Address( i ) ) );
				}
			}

			static Fb2EnumType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;

				return new Fb2EnumType( typeInfo );
			}

			static Fb2EnumType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2EnumType( BADADDR, typeInfoData );
			}
		};
	}
}