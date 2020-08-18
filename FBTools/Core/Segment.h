#pragma once

//#include <windows.h>


#include <pro.h>
#include <segment.hpp>
#include <bytes.hpp>
#include <kernwin.hpp>
#include <funcs.hpp>


#include "../Util/Common.h"

namespace core
{
	bool InSegment( ea_t address, segment_t* lpSegment )
	{
		if ( address > lpSegment->start_ea && address < lpSegment->end_ea )
			return true;

		return false;
	}

	void FixSegmentPointers( )
	{

		auto fFixPointer = []( ea_t address ) -> bool
		{

#ifdef __EA64__

			if ( !create_data( address, qword_flag( ), 8, BADNODE ) )
			{
				msg( "[VMTFix] Failed to write value to address [0x%p]\n", address );
				return false;
			}

			if ( !create_data( address, qword_flag( ) | off_flag( ), 8, BADNODE ) )
			{
				msg( "[VMTFix] Failed to write value to address [0x%p]\n", address );
				return false;
			}

			


#else
			if ( !create_data( address, dword_flag( ), 4, BADNODE ) )
			{
				msg( "[VMTFix] Failed to write value to address [0x%p]\n", address );
				return false;
			}

			if ( !create_data( address, dword_flag( ) | off_flag( ), 4, BADNODE ) )
			{
				msg( "[VMTFix] Failed to write value to address [0x%p]\n", address );
				return false;
			}
#endif

			auto Address = Util::ReadEA( address );


			auto pSegment = getseg( Address );

			if ( pSegment &&
				 ( pSegment->perm & SEGPERM_EXEC ) )
			{
				add_func( Address );
			}

			return true;
		};


		size_t Fixed = 0;

		for ( segment_t* pSegment = get_first_seg( ); pSegment; pSegment = get_next_seg( pSegment->start_ea ) )
		{
			if ( !pSegment )
				break;

			qstring Name;

			get_segm_name( &Name, pSegment );

			if ( ( pSegment->perm & SEGPERM_EXEC ) /*&& !( seg->perm & SEGPERM_READ )*/ )
				continue;

			msg( "[VMTFix] Scanning segment [%s] with size [0x%X]\n", Name.c_str( ), pSegment->size( ) );

			if ( !( pSegment->perm & SEGPERM_READ ) )
			{
				msg( "\t[VMTFix] Segment [%s] not readable\n", Name.c_str( ) );
			}



			uint8_t* pBuffer = new uint8_t[pSegment->size( )]( );
			if ( get_bytes( pBuffer, pSegment->size( ), pSegment->start_ea ) == -1 )
			{
				//msg( "[VMTFix] Failed to get segment data for [%s]\nUsing fallback method!\n", Name.c_str( ) );

				for ( ea_t address = pSegment->start_ea; address <= ( pSegment->end_ea - sizeof( ea_t ) ); address += sizeof( ea_t ) )
				{
#ifdef __EA64__
					ea_t ptr = get_dword( address );
#else
					ea_t ptr = get_qword( address );
#endif

					if ( ptr >= inf.min_ea && ptr <= inf.max_ea )
					{
						msg( "[VMTFix] Found ptr using fallback method [0x%p] at [0x%p]\n", ptr, address );

						if ( !fFixPointer( address ) )
						{
							msg( "[VMTFix] Failed to write value to address [0x%p]\n", address );
							continue;
						}


						Fixed++;

					}

				}

			}
			else
			{
				msg( "[VMTFix] Segment [%s] read into buffer\n", Name.c_str( ) );

				ea_t SegmentEnd = pSegment->size( ) - sizeof( ea_t );

				for ( ea_t f = 0; f <= SegmentEnd; f += sizeof( ea_t ) )
				{
					ea_t ptr = *reinterpret_cast< ea_t* >( pBuffer + f );

					if ( ptr >= inf.min_ea && ptr <= inf.max_ea )
					{
						ea_t realAddr = pSegment->start_ea + f;

						if ( !fFixPointer( realAddr ) )
						{
							msg( "[VMTFix] Failed to write value to address [0x%p]\n", realAddr );
							continue;
						}


						Fixed++;
					}

				}

				
			}

			delete[] pBuffer;

			if ( pSegment == get_last_seg( ) )
				break;
		}


		msg( "[VMTFix] Fixed %d pointers\n", Fixed );


	};
}



