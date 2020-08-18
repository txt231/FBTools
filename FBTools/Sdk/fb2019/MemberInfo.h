#pragma once

#include <ida.hpp>

#include "../../Util/MemoryPointer.h"

#include "MemberInfoFlags.h"


namespace fb2019
{
	class MemberInfo
	{
	public:

#pragma pack( push, 2 )
		class MemberInfoData
		{
		public:
			Util::MemoryString m_pName; //0x0000 const char*
			uint32_t m_UnkHash; //0x0008
			fb2019::MemberInfoFlags m_Flags; //0x000C
		}; //0x000E
#pragma pack( pop )

		Util::MemoryPointer<MemberInfoData> m_pData; //0x0000 MemberInfoData*
	}; //0x0008
}