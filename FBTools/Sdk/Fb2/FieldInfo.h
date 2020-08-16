#pragma once

#include "TypeInfo.h"

namespace fb2
{
	class FieldInfo
		: public MemberInfo
	{
	public:

		class FieldInfoData
			: public MemberInfo::MemberInfoData
		{
		public:
			uint16_t m_FieldOffset; //0x0008 / 0x000A
#ifdef __EA64__
			char _0x000C[0x4];
#endif
			union
			{
				Util::MemoryPointer<TypeInfo> m_pFieldType; //0x0008 / 0x0010 TypeInfo*
				uint32_t m_Value; //0x0008 / 0x0010 not sure if this is needed here...
			};
		}; //0x000C / 0x0018

		ea_t m_Vtable; //0x0004 / 0x0008
	}; //0x0008 / 0x0010
}