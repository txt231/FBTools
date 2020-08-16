#pragma once


#include <string>

#include <idp.hpp>

#include "ITypeBase.h"

#include "../Sdk/MemberInfoFlags.h"

namespace Frostbite
{
	class IFbType
		: public ITypeBase
	{
	public:
		virtual ~IFbType( ) = default;

		virtual bool GetName( std::string& ) = 0;

		virtual bool GetFlags( fb::MemberInfoFlags& flags) = 0;

		virtual ea_t GetModuleAddress( ) = 0;
		virtual bool GetModuleName( std::string& ) = 0;

		virtual ea_t GetDefaultInstance( ) = 0;

		virtual int32_t GetSize( ) = 0;

		virtual int32_t GetAlignment( ) = 0;

		virtual int32_t GetFieldCount( ) = 0;

	};
}