#pragma once

#include "Fb2018Type.h"

#include "../IFbField.h"

#include "../../Sdk/fb2017/ActionTypeInfo.h"

#include "Fb2017FieldType.h"

namespace Frostbite
{
	namespace Fb2017
	{
		class Fb2017ActionType
			: public Fb2017Type
		{
		public:
			Fb2017ActionType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2017Type( typeInfo, typeData )
			{

				//ReadFields( );
			}

			~Fb2017ActionType( )
			{
			}

			//std::vector<Fb2018FieldType*> m_Fields;

			/*
			virtual bool GetFields( std::vector<IFbField*>& outFields ) override
			{
				for ( auto* pField : m_Fields )
					outFields.push_back( pField );


				return outFields.size( ) > 0;
			}
			*/

			/*
			virtual ea_t GetDefaultInstance( ) override
			{
				if ( !this->IsValid( ) )
					return BADADDR;

				Util::MemoryPointer<fb2018::ValueTypeInfo::ValueTypeInfoData> TypeDataRef( m_TypeData );

				fb2018::ValueTypeInfo::ValueTypeInfoData* pData = TypeDataRef;

				if ( !pData )
					return BADADDR;

				if ( pData->m_pDefaultValue == 0 )
					return BADADDR;

				return pData->m_pDefaultValue;
			}
			*/
			virtual bool GetName( std::string& outString ) override
			{
				if ( !this->IsValid( ) )
					return false;

				Util::MemoryPointer<fb2017::ActionTypeInfo::ActionTypeInfoData> TypeDataRef( m_TypeData );

				fb2017::ActionTypeInfo::ActionTypeInfoData* pData = TypeDataRef;

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
					auto Pos = NameString.find_first_of( '(' );

					if ( Pos != std::string::npos )
						outString += NameString.substr( 0, Pos );
					else
						outString += NameString;
				}

				return true;
			}

			static Fb2017ActionType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;

				return new Fb2017ActionType( typeInfo );
			}

			static Fb2017ActionType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2017ActionType( BADADDR, typeInfoData );
			}
		};
	}
}