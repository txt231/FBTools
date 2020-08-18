#pragma once

#include "Fb2017Type.h"

#include "../IFbField.h"

#include "../../Sdk/fb2017/EnumTypeInfo.h"

#include "Fb2017EnumFieldType.h"

namespace Frostbite
{
	namespace Fb2017
	{
		class Fb2017EnumType
			: public Fb2017Type
		{
		public:
			Fb2017EnumType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2017Type( typeInfo, typeData )
			{

				ReadValues( );
			}

			~Fb2017EnumType( )
			{
				for ( auto* pField : m_Values )
					delete pField;

				m_Values.clear( );
			}

			std::vector<Fb2017EnumFieldType*> m_Values;


			void ReadValues( )
			{
				if ( !this->IsValid( ) )
					return;

				if ( this->GetFieldCount( ) == 0 )
					return;

				Util::MemoryPointer<fb2017::EnumTypeInfo::EnumTypeInfoData> TypeDataRef( m_TypeData );

				fb2017::EnumTypeInfo::EnumTypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return;

				if ( pData->m_pFields.m_Ptr == 0 )
					return;

				auto FieldCount = this->GetFieldCount( );
				for ( auto i = 0; i < FieldCount; i++ )
				{
					auto* pEnum = new Fb2017EnumFieldType( pData->m_pFields.Address( i ) );

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


			

			static Fb2017EnumType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;

				return new Fb2017EnumType( typeInfo );
			}

			static Fb2017EnumType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2017EnumType( BADADDR, typeInfoData );
			}
		};
	}
}