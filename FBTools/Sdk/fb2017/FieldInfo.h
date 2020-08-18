#pragma once

#include "TypeInfo.h"

namespace fb2017
{

	class FieldInfo
		: public MemberInfo
	{
	public:

		class FieldInfoData
			: public MemberInfo::MemberInfoData
		{
		public:
			uint16_t m_FieldOffset; //0x0012
#ifdef __EA64__
			char _0x0014[0x4];
#endif
			union
			{
				Util::MemoryPointer<TypeInfo> m_pFieldType; //0x0018 TypeInfo*
				uint32_t m_Value; //0x0018
			};
		}; //0x0020

	}; //0x0008
}