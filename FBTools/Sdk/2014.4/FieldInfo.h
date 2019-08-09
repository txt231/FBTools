#pragma once

#include "TypeInfo.h"

namespace fb
{

	class FieldInfo
		: public MemberInfo
	{
	public:

		class FieldInfoData
			: public MemberInfo::MemberInfoData
		{
		public:
			uint16_t m_FieldOffset; //0x000A
			char _0x000C[0x4];
			Util::MemoryPointer<TypeInfo> m_pFieldType; //0x0010 TypeInfo*
		}; //0x0018

		Util::MemoryPointer<TypeInfo> m_pDeclearingType; //0x0010 TypeInfo*
		uint16_t m_FieldIndex; //0x0018
		uint16_t m_AttributeMask; //0x001A
		char _0x001C[4];
	}; //0x0020

}