#pragma once

#include "../../Util/MemoryPointer.h"

#include "../../Sdk/2014.4/EnumTypeInfo.h"

#include "Fb2014_4FieldType.h"

namespace Frostbite
{
	namespace Fb2014_4
	{
		class Fb2014_4EnumFieldType
			: public Fb2014_4FieldType
		{
		public:
			Fb2014_4EnumFieldType( ea_t typeData = BADADDR )
				: Fb2014_4FieldType( BADADDR, typeData )
			{

			}

			ea_t m_TypeData = BADADDR;



			virtual ea_t GetFieldType( ) override
			{
				return BADADDR;
			}

			virtual int32_t GetOffset( ) override
			{
				if ( !this->IsValid( ) )
					return -1;

				Util::MemoryPointer<fb2014_4::FieldInfo::FieldInfoData> TypeDataRef( m_TypeData );

				fb2014_4::FieldInfo::FieldInfoData* pData = TypeDataRef;

				if ( !pData )
					return -1;

				return pData->m_Value;
			}
		};
	}
}