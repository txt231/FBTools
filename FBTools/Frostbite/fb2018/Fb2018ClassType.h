#pragma once


#include "Fb2018Type.h"


#include "Fb2018FieldType.h"


#include "../../Sdk/fb2018/ClassInfo.h"

namespace Frostbite
{
	namespace Fb2018
	{
		class Fb2018ClassType
			: public Fb2018Type
		{
		public:
			Fb2018ClassType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2018Type( typeInfo, typeData )
			{
				ReadFields( );
			}

			~Fb2018ClassType( )
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

				Util::MemoryPointer<fb2018::ClassInfo::ClassInfoData> TypeDataRef( m_TypeData );

				fb2018::ClassInfo::ClassInfoData* pData = TypeDataRef;

				if ( !pData )
					return;

				if ( pData->m_pFields.m_Ptr == 0 )
					return;

				auto FieldCount = this->GetFieldCount( );
				for ( auto i = 0; i < FieldCount; i++ )
				{
					auto* pField = new Fb2018FieldType( BADADDR, pData->m_pFields.Address( i ) );

					if ( !pField->IsValid( ) )
					{
						delete pField;
						continue;
					}

					m_Fields.push_back( pField );
				}
			}

			virtual ea_t GetParentTypeInfo( )
			{
				if ( !this->IsValid( ) )
					return BADADDR;

				Util::MemoryPointer<fb2018::ClassInfo::ClassInfoData> TypeDataRef( m_TypeData );

				fb2018::ClassInfo::ClassInfoData* pData = TypeDataRef;

				if ( !pData )
					return;

				if( pData->m_pSuper.m_Ptr == 0)
					return BADADDR;

				return  pData->m_pSuper.m_Ptr;
			}


			static Fb2018ClassType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;


				return new Fb2018ClassType( typeInfo );
			}

			static Fb2018ClassType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2018ClassType( BADADDR, typeInfoData );
			}
		};
	}
}