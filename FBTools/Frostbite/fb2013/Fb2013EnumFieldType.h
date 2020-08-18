#pragma once

#include "../../Util/MemoryPointer.h"

#include "../../Sdk/fb2013/EnumTypeInfo.h"

#include "Fb2013FieldType.h"

namespace Frostbite
{
	namespace Fb2013
	{
		class Fb2013EnumFieldType
			: public Fb2013FieldType
		{
		public:
			Fb2013EnumFieldType( ea_t typeData = BADADDR )
				: Fb2013FieldType( BADADDR, typeData )
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

				Util::MemoryPointer<fb2013::FieldInfo::FieldInfoData> TypeDataRef( m_TypeData );

				fb2013::FieldInfo::FieldInfoData* pData = TypeDataRef;

				if ( !pData )
					return -1;

				return pData->m_Value;
			}
		};
	}
}