#pragma once

#include <string>

namespace fb
{
	class ModuleInfo
	{
	public:
		Util::MemoryString m_pModuleName; //0x0000 const char*
		Util::MemoryPointer<ModuleInfo> m_pNextModule; //0x0008 ModuleInfo*
		ea_t m_pTestList; //0x0010  void* fb::TestList
	}; //0x0018
}