#pragma once

#include "TypeInfo.h"
#include "FieldInfo.h"

namespace fb2
{
	class EnumTypeInfo
		: public TypeInfo
	{
	public:
		class EnumTypeInfoData
			: public TypeInfo::TypeInfoData
		{
		public:
			Util::MemoryArrayPointer<FieldInfo::FieldInfoData> m_pFields; //0x0010 / 0x0020
		}; //0x0028

		Util::MemoryArrayPointer<Util::MemoryArrayPointer<FieldInfo>> m_ppFields; //0x000C / 0x0018
	}; //0x0010 / 0x0020

}