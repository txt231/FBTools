#pragma once


#include "TypeInfo.h"

#include "FieldInfo.h"

namespace fb2019
{
	class ClassInfo
		: public TypeInfo
	{
	public:
		class ClassInfoData
			: public TypeInfo::TypeInfoData
		{
		public:
			Util::MemoryPointer<ClassInfo> m_pSuper; //0x0038 ClassInfo*
			Util::MemoryArrayPointer<FieldInfo::FieldInfoData> m_pFields; //0x0040 FieldInfo::FieldInfoData*
			ea_t m_pUnknown; //0x0048
			ea_t m_pCreateFunction1; //0x0050
			ea_t m_pCreateCopy; //0x0058
			ea_t m_pUnkVtableFunc1; //0x0060
			ea_t m_pCopy; //0x0050
			ea_t m_pSetNull; //0x0058
			ea_t m_pUnkCmp1; //0x0060
			ea_t m_pUnkCmp2; //0x0068
			ea_t m_pUnkCopy; //0x0068
			ea_t m_pFreeMaybe; //0x0070
		}; //0x0078

		Util::MemoryPointer<ClassInfo> m_pSuper; //0x0028 ClassInfo*
		ea_t m_pDefaultInstance; //0x0030 DataContainer*
		uint16_t m_ClassId; //0x0038
		uint16_t m_LastSubClassId; //0x003A
#ifdef __EA64__
		char _0x004C[0x4];
#endif
		Util::MemoryPointer<ClassInfo> m_pFirstDerivedClass; //0x0050 ClassInfo*
		Util::MemoryPointer<ClassInfo> m_pNextSiblingClass; //0x0058 ClassInfo*
		Util::MemoryArrayPointer<Util::MemoryPointer<FieldInfo>> m_ppFieldInfos; //0x0060 FieldInfo**
		uint32_t m_TotalFieldCount; //0x0068
#ifdef __EA64__
		char _0x006C[0x4];
#endif
	}; //0x0070
}