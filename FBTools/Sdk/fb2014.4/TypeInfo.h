#pragma once

#include "MemberInfo.h"

#include "../ModuleInfo.h"

namespace fb2014_4
{
	class TypeInfo
		: public MemberInfo
	{
	public:

		class TypeInfoData
			: public MemberInfo::MemberInfoData
		{
		public:
			uint16_t m_Size; //0x000A
#ifdef __EA64__
			char _0x000C[4];
#endif
			Util::MemoryPointer<fb::ModuleInfo> m_pModule; //0x0010 ModuleInfo*
			uint16_t m_Alignment; //0x0018
			uint16_t m_FieldCount; //0x001A
#ifdef __EA64__
			char _0x001C[4];
#endif
		}; //0x0020


		Util::MemoryPointer<TypeInfo> m_pNext; //0x0008 TypeInfo*
		uint16_t m_RuntimeId; //0x0010
		uint16_t m_Flags; //0x0012
#ifdef __EA64__
		char _0x0014[0x4];
#endif
	};
}