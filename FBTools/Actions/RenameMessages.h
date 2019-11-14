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

#include "../Sdk/2014.4/TypeInfo.h"
#include "../Sdk/2014.4/ValueTypeInfo.h"

namespace Actions
{

	class RenameMessages
		: public Action::ActionBase
	{
	public:
		RenameMessages( Frostbite::FBType type )
			: ActionBase( "FBTools:RenameMessages",
						  "Rename messages",
						  "Renames message vtables and constructors" )
		{ }

		virtual int activate( action_activation_ctx_t* ctx ) override
		{
			std::vector<std::pair<ea_t, ea_t>> Types;

			if ( !Frostbite::FindTypeInfos( Types ) )
			{
				msg( "[!] Found no typeinfo!?!\n" );
				return 0;
			}

			for ( auto& TypePairs : Types )
			{
				Util::MemoryPointer<fb::TypeInfo::TypeInfoData> TypeDataRef( TypePairs.second );

				fb::TypeInfo::TypeInfoData* pData = TypeDataRef;

				if ( !pData )
					continue;

				auto Type = pData->m_Flags.GetTypeCode( );


				if ( Type != fb::BTE_ValueType )
					continue;

				if ( pData->m_FieldCount != 0 )
					continue;

				msg( "Found possible message!\n" );

				auto Name = pData->m_pName;

				std::string NameString = Name.GetString( );

				if ( NameString.size( ) == 0 )
					continue;

				if ( NameString.find( "Message" ) == std::string::npos )
					continue;

				//Abstract messages have 2 vtables, and doesnt have default value
				bool IsAbstract = NameString.find( "MessageBase" ) != std::string::npos;


				Util::MemoryPointer<fb::ValueTypeInfo::ValueTypeInfoData> ValueTypeDataRef( TypeDataRef.m_Ptr );

				fb::ValueTypeInfo::ValueTypeInfoData* pValueTypeInfoData = ValueTypeDataRef;

				if ( !pValueTypeInfoData )
					continue;


				if ( IsAbstract )
				{
					msg( "[!] Abstract messages not supported!\n" );
					continue;
				}
				else
				{
					msg( "[+] Message defaultvalue at 0x%p\n", pValueTypeInfoData->m_pDefaultValue );


					if ( pValueTypeInfoData->m_pDefaultValue == 0 )
					{
						msg( "[!] Message has no default value!?\n" );
						continue;
					}


					ea_t VtableAddress = get_qword( pValueTypeInfoData->m_pDefaultValue );

					if ( VtableAddress == BADADDR ||
						 VtableAddress == 0 )
						continue;

					set_name( VtableAddress, ( "??_7" + NameString + "@fb@@6B@" ).c_str( ) );

					//This is standard vtable for all messages
					{
						ea_t Vf1 = get_qword( VtableAddress + sizeof( ea_t ) * 0 );

						if ( Vf1 != 0 || Vf1 != BADADDR )
							set_name( Vf1, ( "?GetType@" + NameString + "@fb@@EBEPBVTypeInfo@2@XZ" ).c_str( ) );
					}
					{
						ea_t Vf2 = get_qword( VtableAddress + sizeof( ea_t ) * 1 );

						if ( Vf2 != 0 || Vf2 != BADADDR )
							set_name( Vf2, ( "??_G" + NameString + "@fb@@UAEPAXI@Z" ).c_str( ) );
					}


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

								if ( Operand.addr != 0 && Operand.addr == pValueTypeInfoData->m_pDefaultValue )
								{
									msg( "[+] Found dynamic initializer for default at 0x%p\n", pFunction->start_ea );
									set_name( pFunction->start_ea, ( "fb::_dynamic_initializer_for___default_" + NameString + "__" ).c_str( ), SN_NOCHECK );
									IsDefaultInitializer = true;
									break;
								}

								if ( Operand.value == pValueTypeInfoData->m_Size )
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
					{
						{
							ea_t Vf = get_qword( VtableAddress + sizeof( ea_t ) * 3 );

							if ( Vf != 0 || Vf != BADADDR )
								set_name( Vf, ( "?GetNmInitiator@" + NameString + "@fb@@EBE?AW4Initiator@NetworkableMessage@2@XZ" ).c_str( ), SN_NOCHECK );
						}
						{
							ea_t Vf = get_qword( VtableAddress + sizeof( ea_t ) * 7 );

							if ( Vf != 0 || Vf != BADADDR )
								set_name( Vf, ( "?GeneratedWriteTo@" + NameString + "@fb@@MBEXPAVIBitStreamWrite@2@AAVNetworkableMessageContext@2@@Z" ).c_str( ), SN_NOCHECK );
						}

						{
							ea_t Vf = get_qword( VtableAddress + sizeof( ea_t ) * 8 );

							if ( Vf != 0 || Vf != BADADDR )
								set_name( Vf, ( "?GeneratedReadFrom@" + NameString + "@fb@@MAE_NPAVIBitStreamRead@2@AAVNetworkableMessageContext@2@@Z" ).c_str( ), SN_NOCHECK );
						}

						{
							ea_t Vf = get_qword( VtableAddress + sizeof( ea_t ) * 9 );

							if ( Vf != 0 || Vf != BADADDR )
								set_name( Vf, ( "?GeneratedClone@" + NameString + "@fb@@EBEPAVNetworkableMessage@2@XZ" ).c_str( ), SN_NOCHECK );
						}
					}
				}
			}



			return 0;
		}


		virtual action_state_t update( action_update_ctx_t* ctx ) override
		{
			return Frostbite::s_pFirstTypeInfo == BADADDR ? AST_DISABLE : AST_ENABLE;
		}


	};
}