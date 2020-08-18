#pragma once

#include "TypeInfo.h"
#include "FieldInfo.h"

namespace fb2018
{
	class EnumTypeInfo
		: public TypeInfo
	{
	public:

		/*
		class EnumTypeData
			: public MemberInfo::MemberInfoData
		{
		public:
			uint16_t m_FieldOffset; //0x000A
			char _0x000C[0x4];
			uint32_t m_Value; //0x0010
			char _0x0014[0x4];
		}; //0x0018
		*/

		class EnumTypeInfoData
			: public TypeInfo::TypeInfoData
		{
		public:
			Util::MemoryArrayPointer<FieldInfo::FieldInfoData> m_pFields; //0x0028
		}; //0x0030

		Util::MemoryArrayPointer<Util::MemoryArrayPointer<FieldInfo>> m_ppFields; //0x0038
	}; //0x0040

}