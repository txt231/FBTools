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
}
