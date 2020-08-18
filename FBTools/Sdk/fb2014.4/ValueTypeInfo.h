#pragma once

#include "TypeInfo.h"
#include "FieldInfo.h"

namespace fb2014_4
{
	class ValueTypeInfo
		: public TypeInfo
	{
	public:
		class ValueTypeInfoData
			: public TypeInfo::TypeInfoData
		{
		public:
			ea_t m_pDefaultValue; //0x0020 void*
			Util::MemoryArrayPointer<FieldInfo::FieldInfoData> m_pFields; //0x0028 FieldInfo::FieldInfoData*
		};

		Util::MemoryArrayPointer<Util::MemoryArrayPointer<FieldInfo>> m_ppFields; //0x0018
	};
}