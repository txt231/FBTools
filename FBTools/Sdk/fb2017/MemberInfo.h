#pragma once

#include <ida.hpp>

#include "../../Util/MemoryPointer.h"

#include "MemberInfoFlags.h"


namespace fb2017
{
	class MemberInfo
	{
	public:

#pragma pack( push, 2 )
		class MemberInfoData
		{
		public:
			Util::MemoryString m_pName; //0x0000 const char*
			char _0x0008[0x8]; // Looks like some hash
			fb2017::MemberInfoFlags m_Flags; //0x0010
		}; //0x0012
#pragma pack( pop )

		Util::MemoryPointer<MemberInfoData> m_pData; //0x0000 MemberInfoData*
	}; //0x0008
}