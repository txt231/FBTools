#pragma once

#include "../../Util/MemoryPointer.h"

#include "../../Sdk/fb2018/EnumTypeInfo.h"

#include "Fb2018FieldType.h"

namespace Frostbite
{
	namespace Fb2018
	{
		class Fb2018EnumFieldType
			: public Fb2018FieldType
		{
		public:
			Fb2018EnumFieldType( ea_t typeData = BADADDR )
				: Fb2018FieldType( BADADDR, typeData )
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

				Util::MemoryPointer<fb2018::FieldInfo::FieldInfoData> TypeDataRef( m_TypeData );

				fb2018::FieldInfo::FieldInfoData* pData = TypeDataRef;

				if ( !pData )
					return -1;

				return pData->m_Value;
			}
		};
	}
}