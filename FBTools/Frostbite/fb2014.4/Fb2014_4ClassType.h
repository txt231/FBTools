#pragma once


#include "Fb2014_4Type.h"

namespace Frostbite
{
	namespace Fb2014_4
	{
		class Fb2014_4ClassType
			: public Fb2014_4Type
		{
		public:
			Fb2014_4ClassType( ea_t typeInfo, ea_t typeData = BADADDR )
				: Fb2014_4Type( typeInfo, typeData )
			{
			}


			virtual int32_t GetFieldCount( ) override
			{
				// Assume classes allways have 0 fields, unless some special debug version that doesnt exist in anyones hands...
				return -1;
			}


			static Fb2014_4ClassType* CreateFromTypeInfo( ea_t typeInfo )
			{
				if ( typeInfo == 0 ||
					 typeInfo == BADADDR )
					return nullptr;


				return new Fb2014_4ClassType( typeInfo );
			}

			static Fb2014_4ClassType* CreateFromTypeInfoData( ea_t typeInfoData )
			{
				if ( typeInfoData == 0 ||
					 typeInfoData == BADADDR )
					return nullptr;

				return new Fb2014_4ClassType( BADADDR, typeInfoData );
			}
		};
	}
}