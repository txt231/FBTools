#pragma once

#include <cstddef>
#include <cstdint>

namespace fb
{
	/*
	uint32_t Hash( const char* pString )
	{
		uint32_t Hash = 5381;

		while( *pString )
			Hash = ( ( Hash << 5 ) + Hash ) ^ *pString++; // hash * 33 ^ c

		return Hash;
	}
	*/


	constexpr std::uint32_t Hash( const char* pString, std::uint32_t h = 5381 ) noexcept
	{
		return ( *pString == 0 ) ? h : Hash( pString + 1, ( ( h << 5 ) + h ) ^ static_cast< std::uint32_t >( *pString ) ); //( h * 33 )
	}

	constexpr auto operator"" _FB_Hash( const char* pString, std::size_t ) noexcept
	{
		return fb::Hash( pString );
	}

}

using fb::operator ""_FB_Hash;