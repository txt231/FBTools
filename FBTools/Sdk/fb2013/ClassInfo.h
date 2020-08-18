#pragma once


#include "TypeInfo.h"

#include "FieldInfo.h"

namespace fb2013
{
	class ClassInfo
		: public TypeInfo
	{
	public:
		class ClassInfoData
			: public TypeInfo::TypeInfoData
		{
		public:
			Util::MemoryPointer<ClassInfo> m_pSuper; //0x0020 ClassInfo*
			ea_t m_pCreateFunction; //0x0028
			Util::MemoryArrayPointer<FieldInfo::FieldInfoData> m_pFields; //0x0030 FieldInfo::FieldInfoData*

		}; //0x0038

		Util::MemoryPointer<ClassInfo> m_pSuper; //0x0018 ClassInfo*
		ea_t m_pDefaultInstance; //0x0020 DataContainer*
		uint16_t m_ClassId; //0x0028
		uint16_t m_LastSubClassId; //0x002A
#ifdef __EA64__
		char _0x002C[0x4];
#endif
		Util::MemoryPointer<ClassInfo> m_pFirstDerivedClass; //0x0030 ClassInfo*
		Util::MemoryPointer<ClassInfo> m_pNextSiblingClass; //0x0038 ClassInfo*
		Util::MemoryArrayPointer<Util::MemoryPointer<FieldInfo>> m_ppFieldInfos; //0x0040 FieldInfo**
		uint32_t m_TotalFieldCount; //0x0048
#ifdef __EA64__
		char _0x004C[0x4];
#endif
	}; //0x0050
}