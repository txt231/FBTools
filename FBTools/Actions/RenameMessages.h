#pragma once

#include <unordered_set>

#include <name.hpp>
#include <struct.hpp>
#include <typeinf.hpp>

#include "../ActionBase.h"

#include "../Core/Pattern.h"

#include "../Frostbite/FrostbiteUtil.h"


#include "../Util/Common.h"
#include "../Util/MemoryPointer.h"

namespace Actions
{

	class RenameMessages
		: public Action::ActionBase
	{
	public:
		RenameMessages( Frostbite::FBVersion type )
			: ActionBase( "FBTools:RenameMessages",
						  "Rename messages",
						  "Renames message vtables and constructors" )
			, m_Version( type )
		{ }

		Frostbite::FBVersion m_Version;

		virtual int activate( action_activation_ctx_t* ctx ) override
		{

			if ( !Frostbite::ReadTypeInfos( ) )
			{
				msg( "[!] Found no typeinfo!?!\n" );
				return 0;
			}

			for ( auto* pType : Frostbite::s_FbTypes )
			{
				fb::MemberInfoFlags Flags;

				if ( !pType->GetFlags( Flags ) )
					continue;

				if ( Flags.GetTypeCode( ) != fb::BTE_ValueType )
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


				auto DefaultInstance = pType->GetDefaultInstance( );


				if ( IsAbstract )
				{
					msg( "[!] Abstract messages not supported!\n" );
					continue;
				}
				else
				{
					msg( "[+] Message defaultvalue at 0x%p\n", DefaultInstance );


					if ( DefaultInstance == BADADDR )
					{
						msg( "[!] Message has no default value!?\n" );
						continue;
					}


					ea_t VtableAddress = Util::ReadEA( DefaultInstance );

					if ( VtableAddress == BADADDR ||
						 VtableAddress == 0 )
						continue;

					RenameMessageVtable( VtableAddress, NameString );


					bool IsNetworkableMessage = false;



					std::vector<xrefblk_t> Refrences;

					if ( !Util::XRefsFrom( VtableAddress, Refrences ) )
						continue;

					msg( "[+] Found %i refrences to %s\n", Refrences.size( ), NameString.c_str( ) );

					for ( auto& Ref : Refrences )
					{
						msg( "\t0x%p\n", Ref.from );

						bool IsClone = false;

						auto fIsGenerateClone = [&]( ea_t addr ) -> bool
						{
							if ( addr - VtableAddress < 0x0 )
								return false;

							if ( addr - VtableAddress > 0x58 )
								return false;

							IsNetworkableMessage = true;
							IsClone = true;
							return true;
						};

						//Finds GenerateClone function
						//TODO: This doesnt work cause generateclone function doesnt exist....
						auto fFindGenerateClone = [&]( ea_t addr ) -> bool
						{
							std::vector<xrefblk_t> FunctionRefrences;

							if ( Util::XRefsFrom( addr, FunctionRefrences ) )
							{
								for ( auto& FuncRefrence : FunctionRefrences )
								{
									msg( "0x%p -> 0x%p\n", addr, FuncRefrence.from );

									if ( fIsGenerateClone( FuncRefrence.from ) )
										break;

									bool Found = false;

									std::vector<xrefblk_t> SubRefrences;

									if ( Util::XRefsFrom( FuncRefrence.from, SubRefrences ) )
									{
										for ( auto& SubFuncRefrence : SubRefrences )
										{
											

											if ( !fIsGenerateClone( SubFuncRefrence.from ) )
												continue;

											msg( "\t0x%p -> 0x%p\n", FuncRefrence.from, SubFuncRefrence.from );

											Found = true;
											break;
										}
									}

									if ( Found )
										return true;
								}
							}
							return false;
						};


						auto pFunction = get_func( Ref.from );
						if ( !pFunction )
						{
							msg( "[!] Failed to find function at ref\n" );

							//Quick fix
							fFindGenerateClone( Ref.from - 0x4B );

							continue;
						}

						
						fFindGenerateClone( pFunction->start_ea );

						if ( IsClone )
							continue;

						bool IsDefaultInitializer = false;
						bool IsNetworkCreateFunction = false;

						for ( auto i = Ref.from; i != pFunction->start_ea; i-- )
						{
							insn_t Instruction;

							if ( decode_insn( &Instruction, i ) == 0 )
								continue;

							for ( auto j = 0; j < 3; j++ )
							{
								auto& Operand = Instruction.ops[j];

								if ( Operand.value == 0 )
									continue;

#ifdef _DEBUG_
								qstring String;
								print_operand( &String, i, j );

								msg( "adr 0x%p Op %i = %s\n", i, j, String.c_str( ) );
#endif

								if ( Operand.addr != 0 && Operand.addr == DefaultInstance )
								{
									msg( "[+] Found dynamic initializer for default at 0x%p\n", pFunction->start_ea );
									set_name( pFunction->start_ea, ( "fb::_dynamic_initializer_for___default_" + NameString + "__" ).c_str( ), SN_NOCHECK );
									IsDefaultInitializer = true;
									break;
								}

								if ( Operand.value == pType->GetSize( ) )
								{
									msg( "[+] Found network create at 0x%p\n", pFunction->start_ea );
									set_name( pFunction->start_ea, ( "fb::create" + NameString ).c_str( ), SN_NOCHECK );
									IsNetworkCreateFunction = true;
									break;
								}

								if ( IsDefaultInitializer || IsNetworkCreateFunction )
									break;
							}

							if ( IsDefaultInitializer || IsNetworkCreateFunction )
								break;
						}

						if ( IsDefaultInitializer || IsNetworkCreateFunction )
							continue;


						msg( "[+] Found real constructor at 0x%p\n", pFunction->start_ea );

						//??0NetworkCreatePlayerMessage@fb@@QAE@XZ

						set_name( pFunction->start_ea, ( "??0" + NameString + "@fb@@QAE@XZ" ).c_str( ), SN_NOCHECK );
					}

					if ( IsNetworkableMessage )
						RenameNetworkableMessageVtable( VtableAddress, NameString );
				}
			}

			return 0;
		}


		virtual action_state_t update( action_update_ctx_t* ctx ) override
		{
			return Frostbite::s_FbTypes.size( ) == 0 ? AST_DISABLE : AST_ENABLE;
		}

		void Fb2_RenameMessageVtable( ea_t vtableAddress, std::string& typeName )
		{
			{
				ea_t Vf = Util::ReadEA( vtableAddress + sizeof( ea_t ) * 3 );

				if ( Vf != 0 &&
					 Vf != BADADDR )
					set_name( Vf, ( "?GetNmInitiator@" + typeName + "@fb@@EBE?AW4Initiator@NetworkableMessage@2@XZ" ).c_str( ), SN_NOCHECK );
			}
			{
				ea_t Vf = Util::ReadEA( vtableAddress + sizeof( ea_t ) * 6 );

				if ( Vf != 0 &&
					 Vf != BADADDR )
					set_name( Vf, ( "?GeneratedWriteTo@" + typeName + "@fb@@MBEXPAVIBitStreamWrite@2@AAVNetworkableMessageContext@2@@Z" ).c_str( ), SN_NOCHECK );
			}
			{
				ea_t Vf = Util::ReadEA( vtableAddress + sizeof( ea_t ) * 7 );

				if ( Vf != 0 &&
					 Vf != BADADDR )
					set_name( Vf, ( "?GeneratedReadFrom@" + typeName + "@fb@@MAE_NPAVIBitStreamRead@2@AAVNetworkableMessageContext@2@@Z" ).c_str( ), SN_NOCHECK );
			}
			{
				ea_t Vf = Util::ReadEA( vtableAddress + sizeof( ea_t ) * 8 );

				if ( Vf != 0 &&
					 Vf != BADADDR )
					set_name( Vf, ( "?GeneratedClone@" + typeName + "@fb@@EBEPAVNetworkableMessage@2@XZ" ).c_str( ), SN_NOCHECK );
			}
		}

		void Fb2013_RenameMessageVtable( ea_t vtableAddress, std::string& typeName )
		{
			{
				ea_t Vf = Util::ReadEA( vtableAddress + sizeof( ea_t ) * 3 );

				if ( Vf != 0 &&
					 Vf != BADADDR )
					set_name( Vf, ( "?GetNmInitiator@" + typeName + "@fb@@EBE?AW4Initiator@NetworkableMessage@2@XZ" ).c_str( ), SN_NOCHECK );
			}
			{
				ea_t Vf = Util::ReadEA( vtableAddress + sizeof( ea_t ) * 7 );

				if ( Vf != 0 &&
					 Vf != BADADDR )
					set_name( Vf, ( "?GeneratedWriteTo@" + typeName + "@fb@@MBEXPAVIBitStreamWrite@2@AAVNetworkableMessageContext@2@@Z" ).c_str( ), SN_NOCHECK );
			}

			{
				ea_t Vf = Util::ReadEA( vtableAddress + sizeof( ea_t ) * 8 );

				if ( Vf != 0 &&
					 Vf != BADADDR )
					set_name( Vf, ( "?GeneratedReadFrom@" + typeName + "@fb@@MAE_NPAVIBitStreamRead@2@AAVNetworkableMessageContext@2@@Z" ).c_str( ), SN_NOCHECK );
			}

			{
				ea_t Vf = Util::ReadEA( vtableAddress + sizeof( ea_t ) * 9 );

				if ( Vf != 0 &&
					 Vf != BADADDR )
					set_name( Vf, ( "?GeneratedClone@" + typeName + "@fb@@EBEPAVNetworkableMessage@2@XZ" ).c_str( ), SN_NOCHECK );
			}
		}

		void RenameNetworkableMessageVtable( ea_t vtableAddress, std::string& typeName )
		{
			switch ( m_Version )
			{
			case Frostbite::FBT_Fb2:
				Fb2_RenameMessageVtable( vtableAddress, typeName );
				break;

			case Frostbite::FBT_Fb2013:
			case Frostbite::FBT_Fb2014:
				Fb2013_RenameMessageVtable( vtableAddress, typeName );
			}
		}

		void RenameMessageVtable( ea_t vtableAddress, std::string& typeName )
		{
			// Set vtable name
			set_name( vtableAddress, ( "??_7" + typeName + "@fb@@6B@" ).c_str( ) );

			// This is standard vtable for all messages
			{
				ea_t Vf1 = Util::ReadEA( vtableAddress + sizeof( ea_t ) * 0 );

				if ( Vf1 != 0 &&
					 Vf1 != BADADDR )
					set_name( Vf1, ( "?GetType@" + typeName + "@fb@@EBEPBVTypeInfo@2@XZ" ).c_str( ) );
			}
			{
				ea_t Vf2 = Util::ReadEA( vtableAddress + sizeof( ea_t ) * 1 );

				if ( Vf2 != 0 &&
					 Vf2 != BADADDR )
					set_name( Vf2, ( "??_G" + typeName + "@fb@@UAEPAXI@Z" ).c_str( ) );
			}
		}
	};
}