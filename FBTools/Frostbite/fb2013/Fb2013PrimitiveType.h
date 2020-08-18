#pragma once

#include "Fb2013Type.h"

namespace Frostbite
{
	namespace Fb2013
	{
		class Fb2013PrimitiveType
			: public Fb2013Type
		{
		public:
			Fb2013PrimitiveType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2013Type( typeInfo, typeData )
			{
			}


			static Fb2013PrimitiveType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;


				return new Fb2013PrimitiveType( typeInfo );
			}

			static Fb2013PrimitiveType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2013PrimitiveType( BADADDR, typeInfoData );
			}
		};
	}
}