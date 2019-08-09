#pragma once

#include <ida.hpp>
#include <idp.hpp>
#include <search.hpp>
#include <allins.hpp>

#include <vector> 

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


		inline uint8_t CharToByte( char Char )
		{
			if ( Char >= '0' && Char <= '9' )
				return Char - '0';

			if ( Char >= 'A' && Char <= 'F' )
				return Char - 'A' + 0xA;

			if ( Char >= 'a' && Char <= 'f' )
				return Char - 'a' + 0xA;

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

					info.m_Bytes.push_back( 0xFF );
					info.m_Mask.push_back( 0 );

					if ( pFormattedSig[1] == '?' )
						pFormattedSig += 2;
					else
						pFormattedSig++;
					continue;
				}


				info.m_Bytes.push_back( ( CharToByte( pFormattedSig[0] ) << 4 ) + CharToByte( pFormattedSig[1] ) );
				info.m_Mask.push_back( 0xFF );

				pFormattedSig += 2;
			}



		}
	}

	ea_t ScanPattern( PatternInternal::PatternInfo& info )
	{

		/*
		{

			compiled_binpat_vec_t Test;

			if ( parse_binpat_str( &Test,
								   0,
								   "48 C7 43 ? ? ? ? ? 48 8B 05 ? ? ? ? 48 89 43 08",
								   16 ) )
			{
				msg( "[+] Parsed test pattern to:\n" );



				for ( auto& BinPat : Test )
				{
					msg( "\tBytes:\n" );


					for ( auto& Byte : BinPat.bytes )
						msg( "%02X ", Byte );

					msg( "\n" );

					msg( "\tMask:\n" );


					for ( auto& Byte : BinPat.mask )
						msg( "%02X(%c) ", Byte, ( char )Byte );

					msg( "\n" );
				}
			}
			else
			{
				msg( "[!] Failed to parse test pattern!\n" );
			}
		}
		*/

		for ( segment_t* pSegment = get_first_seg( ); pSegment; pSegment = get_next_seg( pSegment->start_ea ) )
		{
			if ( !pSegment )
				break;


			msg( "[+] Searching between 0x%p -> 0x%p | HasMask = %i\n", pSegment->start_ea, pSegment->end_ea, info.m_HasMask );

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
			return Result;



		if ( Info.m_RelativeStart != -1 )
		{
			auto RelativeAddress = Result + Info.m_RelativeStart;

			auto RelativeSize = Info.m_RelativeEnd - Info.m_RelativeStart;

			if ( RelativeSize == 4 || Info.m_RelativeEnd == -1 )
			{
				int32_t RelativeOffset = get_dword( RelativeAddress );

				Result = RelativeAddress + 4 + RelativeOffset;
			}
			else if ( RelativeSize == 2 )
			{
				int16_t RelativeOffset = get_word( RelativeAddress );

				Result = RelativeAddress + 2 + RelativeOffset;
			}
			else if ( RelativeSize == 1 )
			{
				int8_t RelativeOffset = get_byte( RelativeAddress );

				Result = RelativeAddress + 1 + RelativeOffset;
			}
		}

		return Result;
	}


}
