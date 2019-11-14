#pragma once

#include <cstdint>

#include "BasicTypesEnum.h"

namespace fb
{
	enum TypeCategoryEnum
	{
		TCE_NotApplicable,
		TCE_Class,
		TCE_ValueType,
		TCE_PrimitiveType
	};

	enum MemberTypeEnum
	{
		MTE_Field,
		MTE_TypeInfo
	};

	struct MemberInfoFlags
	{
		enum Values
		{
			MIFV_MemberTypeMask = 0x3,

			MIFV_TypeCategoryShift = 0x2,
			MIFV_TypeCategoryMask = 0x3,

			MIFV_TypeCodeShift = 0x4,
			MIFV_TypeCodeMask = 0x1F,
		};

		enum Flags
		{
			MIFF_Metadata = ( 1 << 11),
			MIFF_Homogeneous = (1 << 12),
			MIFF_AllwaysPersist = (1 << 13),
			MIFF_Exposed = ( 1 << 13 ),
			MIFF_LayoutImmutable = ( 1 << 14 ),
			MIFF_Blittable = ( 1 << 15 ),
		};

		uint16_t m_Flags; //0x0000



		MemberTypeEnum GetMemberType( )
		{
			return static_cast< MemberTypeEnum >( m_Flags & Values::MIFV_MemberTypeMask );
		}

		TypeCategoryEnum GetTypeCategory( )
		{
			return static_cast< TypeCategoryEnum >( ( m_Flags >> Values::MIFV_TypeCategoryShift ) & Values::MIFV_TypeCategoryMask );
		}

		BasicTypesEnum GetTypeCode( )
		{
			return static_cast< BasicTypesEnum >( ( m_Flags >> Values::MIFV_TypeCodeShift ) & Values::MIFV_TypeCodeMask );
		}

		bool Metadata( )
		{
			return m_Flags & Flags::MIFF_Metadata;
		}

		bool Homogenous( )
		{
			return m_Flags & Flags::MIFF_Homogeneous;
		}

		bool AllwaysPersist( )
		{
			return m_Flags & Flags::MIFF_AllwaysPersist;
		}

		bool Exposed( )
		{
			return m_Flags & Flags::MIFF_Exposed;
		}

		bool LayoutImmutable( )
		{
			return m_Flags & Flags::MIFF_LayoutImmutable;
		}

		bool Blittable( )
		{
			return m_Flags & Flags::MIFF_Blittable;
		}

	}; //0x0002
}