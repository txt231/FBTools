#pragma once

#include <ida.hpp>
#include <idp.hpp>
#include <search.hpp>
#include <allins.hpp>



#include <vector> 


#include "../Util/Common.h"

namespace Core
{
	namespace PatternInternal
	{
		struct PatternInfo
		{
			std::vector<uint8_t> m_Bytes;
			std::vector<uint8_t> m_Mask;
			int32_t m_RelativeStart = -1;
			int32_t m_RelativeEnd = -1;
			bool m_HasMask = false;
		};


		// Stolen from some stackoverflow
		inline uint8_t CharToByte( char value )
		{
			if ( value >= '0' && value <= '9' )
				return value - '0';

			if ( value >= 'A' && value <= 'F' )
				return value - 'A' + 0xA;

			if ( value >= 'a' && value <= 'f' )
				return value - 'a' + 0xA;

			return 0;
		}

		void ParsePattern( const char* pFormattedSig, PatternInfo& info )
		{
			size_t Index = 0;

			// 48 8B 05 [ ?? ?? ?? ?? ]
			while ( *pFormattedSig )
			{
				if ( *pFormattedSig == ' ' )
				{
					pFormattedSig++;
					continue;
				}


				if ( *pFormattedSig == '[' || *pFormattedSig == '(' )
				{
					info.m_RelativeStart = info.m_Bytes.size( );

					pFormattedSig++;
					continue;
				}

				if ( *pFormattedSig == ']' || *pFormattedSig == ')' )
				{
					pFormattedSig++;
					info.m_RelativeEnd = info.m_Bytes.size( );
					continue;
				}


				if ( *pFormattedSig == '?' )
				{
					info.m_HasMask = true;

					info.m_Bytes.push_back( 0x00 );
					info.m_Mask.push_back( 0x00 );

					if ( pFormattedSig[1] == '?' )
						pFormattedSig += 2;
					else
						pFormattedSig++;
					continue;
				}


				info.m_Bytes.push_back( ( CharToByte( pFormattedSig[0] ) << 4 ) | CharToByte( pFormattedSig[1] ) );
				info.m_Mask.push_back( 0xFF );

				pFormattedSig += 2;
			}
		}
	}

	ea_t ScanPattern( PatternInternal::PatternInfo& info )
	{

		for ( segment_t* pSegment = get_first_seg( ); pSegment; pSegment = get_next_seg( pSegment->start_ea ) )
		{
			if ( !pSegment )
				break;

#ifdef _DEBUG
			msg( "[+] Searching between 0x%p -> 0x%p | HasMask = %i\n", pSegment->start_ea, pSegment->end_ea, info.m_HasMask );
#endif


			auto Result = bin_search2( pSegment->start_ea,
									   pSegment->end_ea,
									   info.m_Bytes.data( ),
									   info.m_HasMask ? info.m_Mask.data( ) : nullptr,
									   info.m_Bytes.size( ),
									   BIN_SEARCH_FORWARD );



			if ( Result != BADADDR )
				return Result;


			if ( pSegment == get_last_seg( ) )
				break;
		}

		return BADADDR;
	}

	ea_t FindPattern( const char* pFormattedSig )
	{
		PatternInternal::PatternInfo Info;

		PatternInternal::ParsePattern( pFormattedSig, Info );

		auto Result = ScanPattern( Info );

		if ( Result == BADADDR )
			return BADADDR;



		if ( Info.m_RelativeStart != -1 )
		{
			auto RelativeAddress = Result + Info.m_RelativeStart;

			auto RelativeSize = Info.m_RelativeEnd - Info.m_RelativeStart;

			if ( Info.m_RelativeEnd == -1 )
				RelativeSize = 4;

			Result = Util::ResolvePtr( RelativeAddress, RelativeSize );

		}

		return Result;
	}


}
