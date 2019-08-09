#pragma once

#include <string>

#include <idp.hpp>

#include "../Sdk/MemberInfoFlags.h"


namespace Frostbite
{
	class TypeinfoBase
	{
	public:
		virtual void GetName( std::string& ) = 0;

		virtual fb::MemberInfoFlags GetFlags( ) = 0;

		virtual void GetModuleName( std::string& ) = 0;


		virtual ea_t GetDefaultInstance( ) = 0;

	};
}