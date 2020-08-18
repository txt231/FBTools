#pragma once

#include "TypeInfo.h"
#include "FieldInfo.h"

namespace fb2017
{
	class ActionTypeInfo
		: public TypeInfo
	{
	public:
		class ActionParamData
		{
		public:
			Util::MemoryString m_pName; //0x0000 const char*
			Util::MemoryPointer<TypeInfo> m_pFieldType; //0x0008 TypeInfo*
			uint32_t m_IsReturnValue; //0x0010
			char _0x0014[0xC]; // 3*uint32_t
		}; //0x0020


		class ActionTypeInfoData
			: public TypeInfo::TypeInfoData
		{
		public:
			ea_t m_pActionFunction1_ppResult; //0x0040
			ea_t _0x0030; //0x0048
			ea_t m_pActionFunction2; //0x0050
			ea_t m_pActionFunction3_pResult; //0x0058
			ea_t _0x0048; //0x0060
			Util::MemoryArrayPointer<ActionParamData> m_pFields; //0x0068 ActionFieldData*
		};

		ea_t m_pUnk; //0x0028
	}; //0x0030
}