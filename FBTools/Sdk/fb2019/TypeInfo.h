#pragma once

#include "MemberInfo.h"

#include "../ModuleInfo.h"

namespace fb2019
{
	class TypeInfo
		: public MemberInfo
	{
	public:

		class TypeInfoData
			: public MemberInfo::MemberInfoData
		{
		public:
			uint16_t m_Size; //0x000E
			uint8_t m_Md5Hash[0x10]; //0x0010
			Util::MemoryPointer<fb::ModuleInfo> m_pModule; //0x0020 ModuleInfo*
			Util::MemoryPointer<TypeInfo> m_pArrayType; //0x0028 TypeInfo*
			uint16_t m_Alignment; //0x0030
			uint16_t m_FieldCount; //0x0032
#ifdef __EA64__
			char _0x0034[4];
#endif
		}; //0x0038


		Util::MemoryPointer<TypeInfo> m_pNext; //0x0008 TypeInfo*
		uint8_t m_Md5NameHash[0x10]; //0x0010
		uint16_t m_RuntimeId; //0x0020
		uint16_t m_Flags; //0x0022
#ifdef __EA64__
		char _0x0024[0x4];
#endif
	}; //0x0028
}