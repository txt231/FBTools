#pragma once


namespace Frostbite
{
	class ITypeBase
	{
	public:
		virtual bool IsValid( )
		{
			return false;
		}

		virtual ea_t GetTypeInfo( )
		{
			return BADADDR;
		}

		virtual ea_t GetTypeData( )
		{
			return BADADDR;
		}
	};
}