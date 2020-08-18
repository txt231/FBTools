#pragma once

#include "Fb2014_4Type.h"

#include "../IFbField.h"

#include "../../Sdk/fb2014.4/EnumTypeInfo.h"

#include "Fb2014_4EnumFieldType.h"

namespace Frostbite
{
	namespace Fb2014_4
	{
		class Fb2014_4EnumType
			: public Fb2014_4Type
		{
		public:
			Fb2014_4EnumType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2014_4Type( typeInfo, typeData )
			{

				ReadValues( );
			}

			~Fb2014_4EnumType( )
			{
				for ( auto* pField : m_Values )
					delete pField;

				m_Values.clear( );
			}

			std::vector<Fb2014_4EnumFieldType*> m_Values;


			void ReadValues( )
			{
				if ( !this->IsValid( ) )
					return;

				if ( this->GetFieldCount( ) == 0 )
					return;

				Util::MemoryPointer<fb2014_4::EnumTypeInfo::EnumTypeInfoData> TypeDataRef( m_TypeData );

				fb2014_4::EnumTypeInfo::EnumTypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return;

				if ( pData->m_pFields.m_Ptr == 0 )
					return;

				auto FieldCount = this->GetFieldCount( );
				for ( auto i = 0; i < FieldCount; i++ )
				{
					auto* pEnum = new Fb2014_4EnumFieldType( pData->m_pFields.Address( i ) );

					if ( !pEnum->IsValid( ) )
					{
						delete pEnum;
						continue;
					}

					m_Values.push_back( pEnum );
				}
			}

			virtual bool GetFields( std::vector<IFbField*>& outFields ) override
			{
				for ( auto* pField : m_Values )
					outFields.push_back( pField );


				return outFields.size( ) > 0;
			}
			

			static Fb2014_4EnumType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;

				return new Fb2014_4EnumType( typeInfo );
			}

			static Fb2014_4EnumType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2014_4EnumType( BADADDR, typeInfoData );
			}
		};
	}
}