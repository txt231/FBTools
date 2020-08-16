#pragma once

#include "Fb2Type.h"

namespace Frostbite
{
	namespace Fb2
	{
		class Fb2ClassType
			: public Fb2Type
		{
		public:
			Fb2ClassType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2Type( typeInfo, typeData )
			{
			}


			virtual int32_t GetFieldCount( ) override
			{
				// Assume classes allways have 0 fields, unless some special debug version that doesnt exist in anyones hands...
				return -1;
			}


			static Fb2ClassType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;


				return new Fb2ClassType( typeInfo );
			}

			static Fb2ClassType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2ClassType( BADADDR, typeInfoData );
			}
		};
	}
}