#pragma once

#include "Fb2013Type.h"

#include "../IFbField.h"

#include "../../Sdk/fb2013/EnumTypeInfo.h"

#include "Fb2013EnumFieldType.h"

namespace Frostbite
{
	namespace Fb2013
	{
		class Fb2013EnumType
			: public Fb2013Type
		{
		public:
			Fb2013EnumType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2013Type( typeInfo, typeData )
			{

				ReadValues( );
			}

			~Fb2013EnumType( )
			{
				for ( auto* pField : m_Values )
					delete pField;

				m_Values.clear( );
			}

			std::vector<Fb2013EnumFieldType*> m_Values;


			void ReadValues( )
			{
				if ( !this->IsValid( ) )
					return;

				if ( this->GetFieldCount( ) == 0 )
					return;

				Util::MemoryPointer<fb2013::EnumTypeInfo::EnumTypeInfoData> TypeDataRef( m_TypeData );

				fb2013::EnumTypeInfo::EnumTypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return;

				if ( pData->m_pFields.m_Ptr == 0 )
					return;

				auto FieldCount = this->GetFieldCount( );
				for ( auto i = 0; i < FieldCount; i++ )
				{
					auto* pEnum = new Fb2013EnumFieldType( pData->m_pFields.Address( i ) );

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
			

			static Fb2013EnumType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;

				return new Fb2013EnumType( typeInfo );
			}

			static Fb2013EnumType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2013EnumType( BADADDR, typeInfoData );
			}
		};
	}
}