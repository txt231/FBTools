#pragma once

#include <vector>

#include <ida.hpp>
#include <xref.hpp>



namespace Util
{
	bool XRefsFrom( ea_t ea, std::vector<xrefblk_t>& out, int flags = 0 )
	{
		xrefblk_t Ref;


		if ( !xrefblk_t_first_to( &Ref, ea, flags ) )
			return false;

		do
		{
			out.push_back( Ref );
		} while ( xrefblk_t_next_to(&Ref));


		return out.size( ) > 0;
	}


	uint32_t PtrFlag( )
	{
#ifdef __EA64__
		return qword_flag( );
#else
		return dword_flag( );
#endif
	}

	ea_t ReadEA( ea_t address )
	{
#ifdef __EA64__
		return ( ea_t )get_qword( address );
#else
		return ( ea_t )get_dword( address );
#endif
	}


	ea_t ResolvePtr( ea_t address, size_t size = 4 )
	{
		if ( size == 8 )
		{
			// Just some quick 64 bit absolute ptrs, very rare to find in compiled code
			return ( ea_t ) get_qword( address );
		}
		else if ( size == 4 )
		{
			// 64 bit has relative ptrs, 32 bit has absolute ptrs
#ifdef __EA64__
			int32_t RelativeOffset = get_dword( address );

			return address + 4 + RelativeOffset;
#else
			return Util::ReadEA( address );
#endif
		}
		else if ( size == 2 )
		{
			int16_t RelativeOffset = get_word( address );

			return address + 2 + RelativeOffset;
		}
		else if ( size == 1 )
		{
			int8_t RelativeOffset = get_byte( address );

			return address + 1 + RelativeOffset;
		}

		return BADADDR;
	}
}

