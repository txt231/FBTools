#pragma once

#include "TypeInfo.h"
#include "FieldInfo.h"

namespace fb2017
{
	class ValueTypeInfo
		: public TypeInfo
	{
	public:
		class ValueTypeInfoData
			: public TypeInfo::TypeInfoData
		{
		public:
			ea_t m_pCreateFunction; //0x0040
			ea_t m_pCopyFunction1; //0x0048
			ea_t m_pUnk3; //0x0050
			ea_t m_pCopyFunction2; //0x0058
			ea_t m_pUnk5; //0x0060
			ea_t m_pDefaultValue; //0x0078 void*
			Util::MemoryArrayPointer<FieldInfo::FieldInfoData> m_pFields; //0x0070 FieldInfo::FieldInfoData*
		};

		Util::MemoryArrayPointer<Util::MemoryArrayPointer<FieldInfo>> m_ppFields; //0x0038
		// Some more members here...
	};
}