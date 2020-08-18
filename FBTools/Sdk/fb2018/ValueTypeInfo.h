#pragma once

#include "TypeInfo.h"
#include "FieldInfo.h"

namespace fb2018
{
	class ValueTypeInfo
		: public TypeInfo
	{
	public:
		class ValueTypeInfoData
			: public TypeInfo::TypeInfoData
		{
		public:
			ea_t m_pCreateFunction; //0x0028
			ea_t m_pCopyFunction; //0x0030
			ea_t m_pUnkFunction; //0x0038
			ea_t m_pCopyFunction2; //0x0040
			ea_t m_pDefaultValue; //0x0048 void*
			Util::MemoryArrayPointer<FieldInfo::FieldInfoData> m_pFields; //0x0028 FieldInfo::FieldInfoData*
		};

		Util::MemoryArrayPointer<Util::MemoryArrayPointer<FieldInfo>> m_ppFields; //0x0038
		// Some more members here...
	};
}