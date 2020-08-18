#pragma once

#include <search.hpp>
#include <bytes.hpp>

#include "../ActionBase.h"

#include "../Core/Pattern.h"

#include "../Frostbite/FrostbiteUtil.h"

#include "../Sdk/Hash.h"

namespace Actions
{

	class FindMessageHandlers
		: public Action::ActionBase
	{
	public:
		FindMessageHandlers( Frostbite::FBVersion type )
			: ActionBase( "FBTools:FindMessageHandlers",
						  "Find Message Handlers",
						  "Finds OnMessage functions used to handle messages" )
		{ }


		struct MessageInfo
		{
			std::string m_Name;
			//ea_t m_pTypeInfo;
			//ea_t m_pTypeInfoData;
			bool m_IsAbstract;
			uint32_t m_Hash;

			std::vector<ea_t> m_ScanResults;
		};

		virtual int activate( action_activation_ctx_t* ctx ) override
		{
			std::vector<MessageInfo> Messages;

			{

				if ( !Frostbite::ReadTypeInfos( ) )
				{
					msg( "[!] Found no typeinfo!?!\n" );
					return 0;
				}

				for ( auto* pType : Frostbite::s_FbTypes )
				{

					fb::BasicTypesEnum Type;

					if ( !pType->GetType( Type ) )
						continue;

					if ( Type != fb::BTE_ValueType )
						continue;

					if ( pType->GetFieldCount( ) != 0 )
						continue;

					msg( "Found possible message!\n" );

					std::string NameString;

					if ( !pType->GetName( NameString ) )
						continue;

					if ( NameString.find( "Message" ) == std::string::npos )
						continue;

					//Abstract messages have 2 vtables, and doesnt have default value
					bool IsAbstract = NameString.find( "MessageBase" ) != std::string::npos;


					Messages.push_back( MessageInfo 
										{
											NameString,
											//TypePairs.first,
											//TypePairs.second,
											IsAbstract,
											fb::Hash(NameString.c_str() )
										} );

				}
			}

			msg( "[!] Scanning for handlers\n" );

			for ( auto& Message : Messages )
			{
				ScanForValue( Message.m_Hash, Message.m_ScanResults );

				if ( Message.m_IsAbstract )
				{
					auto MessageHash = fb::Hash( Message.m_Name.substr( 0, Message.m_Name.length( ) - 4 ).c_str( ) );

					ScanForValue( MessageHash, Message.m_ScanResults );
				}
			}

			msg( "[+] Filtering handlers\n" );

			/*
			for ( auto& Message : Messages )
			{
				if( Message.m_ScanResults.size( ) == 0 )
				{ 
					msg( "[!] Failed to find messages for [%s]!", Message.m_Name.c_str( ) );
					msg( "Trying backup scan!" );

				}
			}
			*/

			std::map<ea_t, std::string> FunctionNames;

			for ( auto& Message : Messages )
			{
				std::vector<ea_t> FilteredResults;

				for ( auto& Result : Message.m_ScanResults )
				{
					for ( auto i = Result + 0x8; i != Result - 0x18; i-- )
					{
						insn_t Instruction;

						if ( decode_insn( &Instruction, i ) == 0 )
							continue;

						// This wouldnt work on diffrent arches!
						if ( Instruction.itype != NN_cmp )
							continue;

						bool FoundFiltered = false;

						for ( auto j = 0; j < 3; j++ )
						{
							auto& Operand = Instruction.ops[j];

							if ( Operand.value != Message.m_Hash )
								continue;

							FilteredResults.push_back( i );
							FoundFiltered = true;
						}

						if ( FoundFiltered )
							break;
					}
				}

				Message.m_ScanResults.clear( );

				for ( auto& Result : FilteredResults )
				{
					//Message.m_ScanResults.push_back( Result );

					// Set ida comment
					set_cmt( Result, Message.m_Name.c_str( ), true );

					func_t* pFunction = get_func( Result );

					if ( !pFunction )
					{
						msg( "[!] Found invalid function for onmessage for message [%s] at 0x%p\n", Message.m_Name.c_str( ), Result );
						continue;
					}

					if ( FunctionNames.find( pFunction->start_ea ) == FunctionNames.end( ) )
						FunctionNames[pFunction->start_ea] = Message.m_Name.c_str( );
					else
						FunctionNames[pFunction->start_ea] = FunctionNames[pFunction->start_ea] + "_" + Message.m_Name; // append name
				}
			}

			for ( auto Pair : FunctionNames )
			{
				msg( "[+] Message handler at 0x%p\n", Pair.first );
				set_name( Pair.first, ( "OnMessage_" + Pair.second ).c_str( ), SN_NOCHECK | SN_FORCE );
			}


			//Add decompiler comments maybe?

			return 0;
		}


		virtual action_state_t update( action_update_ctx_t* ctx ) override
		{
			return AST_ENABLE;
		}

		static bool ScanForValue( uint32_t value, std::vector<ea_t>& out )
		{
			for ( segment_t* pSegment = get_first_seg( ); pSegment && pSegment != get_last_seg( ); pSegment = get_next_seg( pSegment->start_ea ) )
			{
				if ( !pSegment )
					break;

				if ( ( pSegment->perm & SEGPERM_EXEC ) )
				{
					ea_t CurrentEa = pSegment->start_ea;

					while ( true )
					{
						if ( ( pSegment->end_ea - CurrentEa) < sizeof( value ) )
							break;

						auto Result = bin_search2( CurrentEa,
												   pSegment->end_ea,
												   reinterpret_cast< uint8_t* >( &value ),
												   nullptr,
												   sizeof( value ),
												   BIN_SEARCH_FORWARD | BIN_SEARCH_NOBREAK );


						if ( Result == BADADDR )
							break;

						out.push_back( Result );

						CurrentEa = Result + 1;
					}
				}

				if ( pSegment == get_last_seg( ) )
					break;
			}

			return out.size( ) > 0;
		}
	};
}