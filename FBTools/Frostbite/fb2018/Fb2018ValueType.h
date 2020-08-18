#pragma once

#include "Fb2018Type.h"

#include "../IFbField.h"

#include "../../Sdk/fb2018/ValueTypeInfo.h"

#include "Fb2018FieldType.h"

namespace Frostbite
{
	namespace Fb2018
	{
		class Fb2018ValueType
			: public Fb2018Type
		{
		public:
			Fb2018ValueType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2018Type( typeInfo, typeData )
			{

				ReadFields( );
			}

			~Fb2018ValueType( )
			{
				for ( auto* pField : m_Fields )
					delete pField;

				m_Fields.clear( );
			}

			std::vector<Fb2018FieldType*> m_Fields;

			void ReadFields( )
			{
				if ( !this->IsValid( ) )
					return;

				if ( this->GetFieldCount( ) == 0 )
					return;

				Util::MemoryPointer<fb2018::ValueTypeInfo::ValueTypeInfoData> TypeDataRef( m_TypeData );

				fb2018::ValueTypeInfo::ValueTypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return;

				if ( pData->m_pFields.m_Ptr == 0 )
					return;

				auto FieldCount = this->GetFieldCount( );
				for ( auto i = 0; i < FieldCount; i++ )
				{
					auto pFieldData = pData->m_pFields.Address( i );

					auto pField = new Fb2018FieldType( BADADDR, pFieldData );

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

				Util::MemoryPointer<fb2018::ValueTypeInfo::ValueTypeInfoData> TypeDataRef( m_TypeData );

				fb2018::ValueTypeInfo::ValueTypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return BADADDR;

				if( pData->m_pDefaultValue == 0 )
					return BADADDR;

				return pData->m_pDefaultValue;
			}


			static Fb2018ValueType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;

				return new Fb2018ValueType( typeInfo );
			}

			static Fb2018ValueType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2018ValueType( BADADDR, typeInfoData );
			}
		};
	}
}