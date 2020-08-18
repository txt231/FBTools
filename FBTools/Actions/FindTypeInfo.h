#pragma once

#include <functional>

#include "../ActionBase.h"

#include "../Core/Pattern.h"

#include "../Frostbite/FrostbiteUtil.h"

//#include "../Frostbite/FBType.h"

namespace Actions
{

	static std::map<Frostbite::FBVersion, std::string> c_FbSigs =
	{
		
		// bf3 release
		{
			Frostbite::FBT_Fb2,
			"3B EB"
			"0F 85 ?? ?? ?? ??"
			"8B 15 [ ?? ?? ?? ?? ]"
		},

		/*
		// bf4, catalyst closed beta...
		//66 FF C0 66 89 51 ?? 48 89 51 ?? 66 89 41 ?? 66 89 05 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ??
		{
			Frostbite::FBT_Fb2013,
			"66 FF C0"					// 00000001405B329C 66 FF C0			 inc	ax
			"66 89 51 ??"				// 00000001405B329F 66 89 51 12			 mov	[rcx+12h], dx
			"48 89 51 ??"				// 00000001405B32A3 48 89 51 18			 mov	[rcx+18h], rdx
			"66 89 41 ??"				// 00000001405B32A7 66 89 41 10			 mov	[rcx+10h], ax
			"66 89 05 ?? ?? ?? ??"		// 00000001405B32AB 66 89 05 FE 8D B4 01 mov	cs:word_1420FC0B0, ax
			"48 8B 05 [ ?? ?? ?? ?? ]"	// 00000001405B32B2 48 8B 05 8F 0D B4 01 mov	rax, cs:g_FirstTypeInfoPtr
		},
		*/

		// bf4
		// 48 85 D2 75 ? 48 89 05 ? ? ? ? 
		{
			Frostbite::FBT_Fb2013,
			"48 85 D2"
			"75 ??"
			"48 89 05 [ ?? ?? ?? ?? ]"
		},

		
		// Catalyst closed beta
		// 48 85 FF 75 ? 48 8B 1D ? ? ? ? 4C 8B 05 ? ? ? ? 
		{
			Frostbite::FBT_Fb2014,
			"48 85 FF"
			"75 ??"
			"48 8B 1D [ ?? ?? ?? ?? ]"
			"4C 8B 05 ?? ?? ?? ??"
		},

		// bf1
		// 48 C7 43 ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 89 43 08
		{ 
			Frostbite::FBT_Fb2014,
			"48 C7 43 ?? ?? ?? ?? ??"
			"48 8B 05 [ ?? ?? ?? ?? ]"
			"48 89 43 08" 
		},


		// bfv fb2018
		// 66 41 39 C1 75 12 48 8B 05 ? ? ? ? 
		{
			Frostbite::FBT_Fb2018,
			"66 41 39 C1"
			"75 12"
			"48 8B 05 [ ?? ?? ?? ?? ]"
		},
	};

	class FindTypeInfoAction
		: public Action::ActionBase
	{
	public:
		FindTypeInfoAction( std::function<void( Frostbite::FBVersion )> callback )
			: ActionBase( "FBTools:FindTypeInfo",
						  "Find FirstTypeInfo",
						  "Finds FirstTypeInfo so you can use other actions..." )
			, m_Callback( callback )
		{
		}

		std::function<void( Frostbite::FBVersion )> m_Callback;

		virtual int activate( action_activation_ctx_t* ctx ) override
		{
			if ( FindBySignature( ) )
				return 0;

			if ( FindManually( ) )
				return 0;

			msg( "[!] Couldnt find typeinfo!\n");
			//Frostbite::ReadTypeInfos( );

			return 0;
		}


		virtual action_state_t update( action_update_ctx_t* ctx ) override
		{
			return AST_ENABLE;
		}

		bool FindBySignature( )
		{
			for ( auto& Pair : c_FbSigs )
			{
				auto Result = Core::FindPattern( Pair.second.c_str( ) );

				if ( Result == BADADDR )
				{
					msg( "[!] Failed to find firsttypeinfo for version [%i]!\n", Pair.first );
					continue;
				}

				msg( "[!] Found firttypeinfo for version [%i] at 0x%p\n", Pair.first, Result );
				Frostbite::s_pFirstTypeInfo = Result;
				Frostbite::s_FbVersion = Pair.first;

				Frostbite::ReadTypeInfos( );

				m_Callback( Pair.first );

				return true;
			}

			return false;
		}



		bool FindManually( )
		{

			return false;
		}
	};
}