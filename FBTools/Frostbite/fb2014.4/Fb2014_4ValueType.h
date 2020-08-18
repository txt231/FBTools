#pragma once

#include "Fb2014_4Type.h"

#include "../IFbField.h"

#include "../../Sdk/fb2014.4/ValueTypeInfo.h"

#include "Fb2014_4FieldType.h"

namespace Frostbite
{
	namespace Fb2014_4
	{
		class Fb2014_4ValueType
			: public Fb2014_4Type
		{
		public:
			Fb2014_4ValueType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2014_4Type( typeInfo, typeData )
			{

				ReadFields( );
			}

			~Fb2014_4ValueType( )
			{
				for ( auto* pField : m_Fields )
					delete pField;

				m_Fields.clear( );
			}

			std::vector<Fb2014_4FieldType*> m_Fields;

			void ReadFields( )
			{
				if ( !this->IsValid( ) )
					return;

				if ( this->GetFieldCount( ) == 0 )
					return;

				Util::MemoryPointer<fb2014_4::ValueTypeInfo::ValueTypeInfoData> TypeDataRef( m_TypeData );

				fb2014_4::ValueTypeInfo::ValueTypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return;

				if ( pData->m_pFields.m_Ptr == 0 )
					return;

				auto FieldCount = this->GetFieldCount( );
				for ( auto i = 0; i < FieldCount; i++ )
				{
					auto* pField = new Fb2014_4FieldType( BADADDR, pData->m_pFields.Address( i ) );

					if ( !pField->IsValid( ) )
					{
						delete pField;
						continue;
					}

					m_Fields.push_back( pField );
				}
			}

			virtual bool GetFields( std::vector<IFbField*>& outFields ) override
			{
				for ( auto* pField : m_Fields )
					outFields.push_back( pField );


				return outFields.size( ) > 0;
			}
			

			virtual ea_t GetDefaultInstance( ) override
			{
				if ( !this->IsValid( ) )
					return BADADDR;

				Util::MemoryPointer<fb2014_4::ValueTypeInfo::ValueTypeInfoData> TypeDataRef( m_TypeData );

				fb2014_4::ValueTypeInfo::ValueTypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return BADADDR;

				if( pData->m_pDefaultValue == 0 )
					return BADADDR;

				return pData->m_pDefaultValue;
			}


			static Fb2014_4ValueType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;

				return new Fb2014_4ValueType( typeInfo );
			}

			static Fb2014_4ValueType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2014_4ValueType( BADADDR, typeInfoData );
			}
		};
	}
}