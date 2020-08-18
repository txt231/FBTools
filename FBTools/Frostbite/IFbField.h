#pragma once



#include <string>

#include <idp.hpp>

#include "../Sdk/MemberInfoFlags.h"


namespace Frostbite
{
	class IFbField
		: public ITypeBase
	{
	public:
		virtual bool GetName( std::string& ) = 0;

		virtual bool GetType( fb::BasicTypesEnum& flags ) = 0;
		virtual bool GetTypeCategory( fb::TypeCategoryEnum& flags ) = 0;
		virtual bool GetMemberType( fb::MemberTypeEnum& flags ) = 0;
		//virtual bool GetFlags( fb::MemberInfoFlags& flags ) = 0;

		virtual ea_t GetFieldType( ) = 0;

		virtual int32_t GetOffset( ) = 0;
	};
}