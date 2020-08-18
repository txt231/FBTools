#pragma once

#include "../MemberInfoFlags.h"

namespace fb2017
{
	struct MemberInfoFlags
		: public fb::MemberInfoFlagsBase
	{
		MemberInfoFlags( )
			: MemberInfoFlagsBase( )
		{
		}

		enum Values
		{
			MIFV_MemberTypeMask = 0x3,

			MIFV_TypeCategoryShift = 0x2,
			MIFV_TypeCategoryMask = 0x7,

			MIFV_TypeCodeShift = 0x5,
			MIFV_TypeCodeMask = 0x1F,
		};

		fb::MemberTypeEnum GetMemberType( )
		{
			return static_cast< fb::MemberTypeEnum >( m_Flags & Values::MIFV_MemberTypeMask );
		}

		fb::TypeCategoryEnum GetTypeCategory( )
		{
			return static_cast< fb::TypeCategoryEnum >( ( m_Flags >> Values::MIFV_TypeCategoryShift ) & Values::MIFV_TypeCategoryMask );
		}

		fb::BasicTypesEnum GetTypeCode( )
		{
			return static_cast< fb::BasicTypesEnum >( ( m_Flags >> Values::MIFV_TypeCodeShift ) & Values::MIFV_TypeCodeMask );
		}

	}; //0x0002
}