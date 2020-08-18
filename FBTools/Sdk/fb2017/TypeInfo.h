#pragma once

#include "MemberInfo.h"

#include "../ModuleInfo.h"

namespace fb2017
{
	class TypeInfo
		: public MemberInfo
	{
	public:

		class TypeInfoData
			: public MemberInfo::MemberInfoData
		{
		public:
			uint16_t m_Size; //0x0012
			uint8_t m_Md5Hash[0x10]; //0x0014
#ifdef __EA64__
			char _0x0024[4];
#endif
			Util::MemoryPointer<fb::ModuleInfo> m_pModule; //0x0028 ModuleInfo*
			Util::MemoryPointer<TypeInfo> m_pArrayType; //0x0030 TypeInfo*
			uint16_t m_Alignment; //0x0038
			uint16_t m_FieldCount; //0x003A
#ifdef __EA64__
			char _0x003C[4];
#endif
		}; //0x0040


		Util::MemoryPointer<TypeInfo> m_pNext; //0x0008 TypeInfo*
		uint8_t m_Md5NameHash[0x10]; //0x0010 in anthem they removed m_pName, but kept the md5 hash and the function that hashes it
		uint16_t m_RuntimeId; //0x0020
		uint16_t m_Flags; //0x0022
#ifdef __EA64__
		char _0x0024[0x4];
#endif
	}; //0x0028
}