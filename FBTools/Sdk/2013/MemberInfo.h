#pragma once

#include <ida.hpp>

#include "../../Util/MemoryPointer.h"

#include "../MemberInfoFlags.h"


namespace fb2013
{
	class MemberInfo
	{
	public:

#pragma pack( push, 2 )
		class MemberInfoData
		{
		public:
			Util::MemoryString m_pName; //0x0000 const char*
			fb::MemberInfoFlags m_Flags; //0x0008
		}; //0x000A
#pragma pack( pop )

		Util::MemoryPointer<MemberInfoData> m_pData; //0x0000 MemberInfoData*
	}; //0x0008
}