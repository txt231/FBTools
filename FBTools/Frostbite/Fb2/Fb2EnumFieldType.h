#pragma once

#include "../../Util/MemoryPointer.h"

#include "../../Sdk/Fb2/EnumTypeInfo.h"

#include "Fb2FieldType.h"

namespace Frostbite
{
	namespace Fb2
	{
		class Fb2EnumFieldType
			: public Fb2FieldType
		{
		public:
			Fb2EnumFieldType( ea_t typeData = BADADDR )
				: Fb2FieldType( BADADDR, typeData )
			{

			}


			virtual ea_t GetFieldType( ) override
			{
				return BADADDR;
			}

			virtual int32_t GetOffset( ) override
			{
				if ( !this->IsValid( ) )
					return -1;

				Util::MemoryPointer<fb2::FieldInfo::FieldInfoData> TypeDataRef( m_TypeData );

				fb2::FieldInfo::FieldInfoData* pData = TypeDataRef;

				if ( !pData )
					return -1;

				return pData->m_Value;
			}
		};
	}
}