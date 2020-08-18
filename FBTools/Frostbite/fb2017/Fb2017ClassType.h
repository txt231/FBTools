#pragma once


#include "Fb2017Type.h"


#include "Fb2017FieldType.h"


#include "../../Sdk/fb2017/ClassInfo.h"

namespace Frostbite
{
	namespace Fb2017
	{
		class Fb2017ClassType
			: public Fb2017Type
		{
		public:
			Fb2017ClassType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2017Type( typeInfo, typeData )
			{
				ReadFields( );
			}

			~Fb2017ClassType( )
			{
				for ( auto* pField : m_Fields )
					delete pField;

				m_Fields.clear( );
			}

			std::vector<Fb2017FieldType*> m_Fields;

			void ReadFields( )
			{
				if ( !this->IsValid( ) )
					return;

				if ( this->GetFieldCount( ) == 0 )
					return;

				Util::MemoryPointer<fb2017::ClassInfo::ClassInfoData> TypeDataRef( m_TypeData );

				fb2017::ClassInfo::ClassInfoData* pData = TypeDataRef;

				if ( !pData )
					return;

				if ( pData->m_pFields.m_Ptr == 0 )
					return;

				auto FieldCount = this->GetFieldCount( );
				for ( auto i = 0; i < FieldCount; i++ )
				{
					auto* pField = new Fb2017FieldType( BADADDR, pData->m_pFields.Address( i ) );

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

				Util::MemoryPointer<fb2017::ClassInfo::ClassInfoData> TypeDataRef( m_TypeData );

				fb2017::ClassInfo::ClassInfoData* pData = TypeDataRef;

				if ( !pData )
					return BADADDR;

				if( pData->m_pSuper.m_Ptr == 0)
					return BADADDR;

				return  pData->m_pSuper.m_Ptr;
			}


			static Fb2017ClassType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;


				return new Fb2017ClassType( typeInfo );
			}

			static Fb2017ClassType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2017ClassType( BADADDR, typeInfoData );
			}
		};
	}
}