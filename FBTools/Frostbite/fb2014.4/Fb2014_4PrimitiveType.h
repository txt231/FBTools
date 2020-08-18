#pragma once

#include "Fb2014_4Type.h"

namespace Frostbite
{
	namespace Fb2014_4
	{
		class Fb2014_4PrimitiveType
			: public Fb2014_4Type
		{
		public:
			Fb2014_4PrimitiveType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2014_4Type( typeInfo, typeData )
			{
			}


			static Fb2014_4PrimitiveType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;


				return new Fb2014_4PrimitiveType( typeInfo );
			}

			static Fb2014_4PrimitiveType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2014_4PrimitiveType( BADADDR, typeInfoData );
			}
		};
	}
}