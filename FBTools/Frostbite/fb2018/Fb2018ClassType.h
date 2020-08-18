#pragma once


#include "Fb2018Type.h"

namespace Frostbite
{
	namespace Fb2018
	{
		class Fb2018ClassType
			: public Fb2018Type
		{
		public:
			Fb2018ClassType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2018Type( typeInfo, typeData )
			{
			}


			virtual int32_t GetFieldCount( ) override
			{
				// Assume classes allways have 0 fields, unless some special debug version that doesnt exist in anyones hands...
				return -1;
			}


			static Fb2018ClassType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;


				return new Fb2018ClassType( typeInfo );
			}

			static Fb2018ClassType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2018ClassType( BADADDR, typeInfoData );
			}
		};
	}
}