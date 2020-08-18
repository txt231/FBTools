#pragma once

#include "../../Util/MemoryPointer.h"

#include "../../Sdk/fb2017/EnumTypeInfo.h"

#include "Fb2017FieldType.h"

namespace Frostbite
{
	namespace Fb2017
	{
		class Fb2017EnumFieldType
			: public Fb2017FieldType
		{
		public:
			Fb2017EnumFieldType( ea_t typeData = BADADDR )
				: Fb2017FieldType( BADADDR, typeData )
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

				Util::MemoryPointer<fb2017::FieldInfo::FieldInfoData> TypeDataRef( m_TypeData );

				fb2017::FieldInfo::FieldInfoData* pData = TypeDataRef;

				if ( !pData )
					return -1;

				return pData->m_Value;
			}
		};
	}
}