#pragma once

#include <unordered_map>

#include <bytes.hpp>

namespace Util
{
	namespace Internal
	{
		class MemoryPointerInfo
		{
		public:
			MemoryPointerInfo( )
			{
			}

			~MemoryPointerInfo( )
			{
				for ( auto& Data : this->m_PointerMap )
				{
					if ( !Data.second.m_pData )
						continue;

					free( Data.second.m_pData );
					Data.second.m_pData = nullptr;
				}

				this->m_PointerMap.clear( );
			}

			struct DataInfo
			{
				void* m_pData;
				size_t m_Size;
			};

			std::unordered_map<ea_t, DataInfo> m_PointerMap;



			template <class T>
			T* CreateData( ea_t ptr )
			{
				if ( ptr == BADADDR )
					return nullptr;

				if ( ptr == 0 )
					return nullptr;

				T* pData = reinterpret_cast< T* >( malloc( sizeof( T ) ) );

				if ( get_bytes( pData, sizeof( T ), ptr ) == -1 )
				{
					free( pData );
					return nullptr;
				}

				m_PointerMap[ptr] = DataInfo{ pData, sizeof( T ) };

				return pData;
			}

			template <class T>
			T* GetData( ea_t ptr )
			{
				if ( ptr == BADADDR )
					return nullptr;

				if ( ptr == 0 )
					return nullptr;

				auto& FindResult = m_PointerMap.find( ptr );

				if ( FindResult == m_PointerMap.end( ) )
					return this->CreateData<T>( ptr );

				if ( FindResult->second.m_Size != sizeof( T ) )
				{
					if ( FindResult->second.m_pData )
						free( FindResult->second.m_pData );

					return this->CreateData<T>( ptr );
				}

				if ( get_bytes( FindResult->second.m_pData, sizeof( T ), ptr ) == -1 )
					return nullptr;

				return reinterpret_cast< T* >( FindResult->second.m_pData );
			}

			static MemoryPointerInfo& GetInstance( )
			{
				static MemoryPointerInfo s_PointerInfo;
				return s_PointerInfo;
			}
		};
	}



	template <class T>
	class MemoryPointer
	{
	public:
		MemoryPointer( )
		{
		}

		MemoryPointer( ea_t ptr )
			: m_Ptr( ptr )
		{
		}

		/*
#ifdef __EA64__
		_declspec( align( 8 ) )
#else
		_declspec( align( 4 ) )
#endif
			*/
		ea_t m_Ptr = BADADDR; //0x0000

		ea_t RawPtr( )
		{
			return this->m_Ptr;
		}

		T* Get( )
		{
			return Internal::MemoryPointerInfo::GetInstance( ).GetData<T>( this->m_Ptr );
		}

		inline void Set( ea_t ptr )
		{
			this->m_Ptr = ptr;
		}

		operator T* ( )
		{
			return this->Get( );
		}

		T* operator->( )
		{
			return this->Get( );
		}
	};


	template <class T>
	class MemoryArrayPointer
		: public MemoryPointer<T>
	{
	public:

		T* At( size_t index )
		{
			return Internal::MemoryPointerInfo::GetInstance( ).GetData<T>( this->m_Ptr + sizeof( T ) * index );
		}

		ea_t Address( size_t index )
		{
			return this->m_Ptr + sizeof( T ) * index;
		}

		T* operator []( size_t index )
		{
			return this->At( index );
		}
	};


	class MemoryString
		: public MemoryArrayPointer<char>
	{
	public:


		std::string GetString( )
		{
			std::string Data = "";
			for ( auto i = 0; true; i++ )
			{
				char Current = *this->At( i );

				if ( Current == '\0' )
					break;

				Data += Current;
			}

			return Data;
		}

	};
}