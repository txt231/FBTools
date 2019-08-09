#pragma once

#include <vector>

#include "ActionBase.h"

namespace Action
{

	class ActionManager
	{
	public:

		~ActionManager( )
		{
			for ( auto& pAction : this->m_Actions )
				delete pAction;


			this->m_Actions.clear( );

		}

		std::vector<ActionBase*> m_Actions;


		void RegisterAction( ActionBase* pAction )
		{
			if ( pAction == nullptr )
				return;

			m_Actions.push_back( pAction );

			pAction->RegisterAction( );
		}


		static ActionManager& GetInstance( )
		{
			static ActionManager s_Instance;

			return s_Instance;
		}
	};

}