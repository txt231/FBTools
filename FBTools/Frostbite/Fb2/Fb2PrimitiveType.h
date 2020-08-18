#pragma once

#include "Fb2Type.h"

namespace Frostbite
{
	namespace Fb2
	{
		class Fb2PrimitiveType
			: public Fb2Type
		{
		public:
			Fb2PrimitiveType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2Type( typeInfo, typeData )
			{
			}


			static Fb2PrimitiveType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;


				return new Fb2PrimitiveType( typeInfo );
			}

			static Fb2PrimitiveType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2PrimitiveType( BADADDR, typeInfoData );
			}
		};
	}
}