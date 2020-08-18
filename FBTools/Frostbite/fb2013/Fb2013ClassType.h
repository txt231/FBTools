#pragma once


#include "Fb2013Type.h"

namespace Frostbite
{
	namespace Fb2013
	{
		class Fb2013ClassType
			: public Fb2013Type
		{
		public:
			Fb2013ClassType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2013Type( typeInfo, typeData )
			{
			}


			virtual int32_t GetFieldCount( ) override
			{
				// Assume classes allways have 0 fields, unless some special debug version that doesnt exist in anyones hands...
				return -1;
			}


			static Fb2013ClassType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;


				return new Fb2013ClassType( typeInfo );
			}

			static Fb2013ClassType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2013ClassType( BADADDR, typeInfoData );
			}
		};
	}
}