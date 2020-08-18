#pragma once

#include "Fb2018Type.h"

namespace Frostbite
{
	namespace Fb2018
	{
		class Fb2018PrimitiveType
			: public Fb2018Type
		{
		public:
			Fb2018PrimitiveType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2018Type( typeInfo, typeData )
			{
			}


			static Fb2018PrimitiveType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;


				return new Fb2018PrimitiveType( typeInfo );
			}

			static Fb2018PrimitiveType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2018PrimitiveType( BADADDR, typeInfoData );
			}
		};
	}
}