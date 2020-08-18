#pragma once

#include "Fb2017Type.h"

namespace Frostbite
{
	namespace Fb2017
	{
		class Fb2017PrimitiveType
			: public Fb2017Type
		{
		public:
			Fb2017PrimitiveType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2017Type( typeInfo, typeData )
			{
			}


			static Fb2017PrimitiveType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;


				return new Fb2017PrimitiveType( typeInfo );
			}

			static Fb2017PrimitiveType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2017PrimitiveType( BADADDR, typeInfoData );
			}
		};
	}
}