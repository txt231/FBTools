#pragma once

#include "Fb2013Type.h"

#include "../IFbField.h"

#include "../../Sdk/fb2013/ValueTypeInfo.h"

#include "Fb2013FieldType.h"

namespace Frostbite
{
	namespace Fb2013
	{
		class Fb2013ValueType
			: public Fb2013Type
		{
		public:
			Fb2013ValueType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2013Type( typeInfo, typeData )
			{
				ReadFields( );
			}

			~Fb2013ValueType( )
			{
				for ( auto* pField : m_Fields )
					delete pField;

				m_Fields.clear( );
			}

			std::vector<Fb2013FieldType*> m_Fields;

			void ReadFields( )
			{
				if ( !this->IsValid( ) )
					return;

				if ( this->GetFieldCount( ) == 0 )
					return;

				Util::MemoryPointer<fb2013::ValueTypeInfo::ValueTypeInfoData> TypeDataRef( m_TypeData );

				fb2013::ValueTypeInfo::ValueTypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return;

				if ( pData->m_pFields.m_Ptr == 0 )
					return;

				auto FieldCount = this->GetFieldCount( );
				for ( auto i = 0; i < FieldCount; i++ )
				{
					auto pFieldData = pData->m_pFields.Address( i );

					auto pField = new Fb2013FieldType( BADADDR, pFieldData );

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

				Util::MemoryPointer<fb2013::ValueTypeInfo::ValueTypeInfoData> TypeDataRef( m_TypeData );

				fb2013::ValueTypeInfo::ValueTypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return BADADDR;

				if( pData->m_pDefaultValue == 0 )
					return BADADDR;

				return pData->m_pDefaultValue;
			}


			static Fb2013ValueType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;

				return new Fb2013ValueType( typeInfo );
			}

			static Fb2013ValueType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2013ValueType( BADADDR, typeInfoData );
			}
		};
	}
}