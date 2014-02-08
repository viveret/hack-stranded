#ifdef _TEST
#include "Manager.hpp"
using namespace SE_Manager;

#include "../S_Engine.hpp"

#include "../utility/Functions.hpp"


SE_Man_Item::SE_Man_Item()
{
	this->Handle_ID = 0;// reserved for errors
}





SE_Man_Manager::SE_Man_Manager()
{

}

SE_Man_Manager::~SE_Man_Manager()
{

}

uint SE_Man_Manager::Get_ID( SE_Man_Item* Which )
{
	uint Ret = 0;

	this->m_Mutex.Wait_For_Lock( );// FILE_LINE.c_str() );
	this->m_Mutex.Lock();

	for( uint i = 0;i < this->Items.size(); i++ )
	{
		if( this->Items[i] == Which )
		{
			Ret = i;
			break;
		}
	}

	this->m_Mutex.Unlock();

	return Ret;
}

SE_Man_Item* SE_Man_Manager::Get_Item( uint Which )
{
	SE_Man_Item* Ret = NULL;

	this->m_Mutex.Wait_For_Lock( ); // FILE_LINE.c_str() );
	this->m_Mutex.Lock();

	for( uint i = 0;i < this->Items.size(); i++ )
	{
		if( this->Items[i]->Handle_ID == Which )
		{
			Ret = this->Items[i];
			break;
		}
	}

	this->m_Mutex.Unlock();

	return Ret;
}

void SE_Man_Manager::Cleanup_Base()
{
	this->m_Mutex.Wait_For_Lock( );// FILE_LINE.c_str() );
	this->m_Mutex.Lock();

	for( uint i = 0;i < this->Items.size(); i++ )
	{
		this->Items[i]->Cleanup();
#ifdef _TEST
		Engine.Mem_Remove( this->Items[i] );
#endif
		delete this->Items[i];
	}
	this->Items.clear();

	this->m_Mutex.Unlock();
}

void SE_Man_Manager::Cleanup()
{
	this->Cleanup_Base();
}

uint SE_Man_Manager::Item_Add_New( SE_Man_Item* What )
{
	What->Handle_ID = this->Gen_ID();
	this->Items.push_back( What );
	return What->Handle_ID;
}

void SE_Man_Manager::Item_Remove( uint Which )
{
	this->m_Mutex.Wait_For_Lock( ); // FILE_LINE.c_str() );
	this->m_Mutex.Lock();

	for( uint i = 0;i < this->Items.size(); i++ )
	{
		if( this->Items[i]->Handle_ID == Which )
		{
			this->Items[i]->Cleanup();
#ifdef _TEST
			Engine.Mem_Remove( this->Items[i] );
#endif

			delete this->Items[i];
			this->Items.erase( this->Items.begin() + i);
			i--;
		}
	}

	this->m_Mutex.Unlock();
}

void SE_Man_Manager::Item_Remove( SE_Man_Item* Which )
{
	this->m_Mutex.Wait_For_Lock( ); // FILE_LINE.c_str() );
	this->m_Mutex.Lock();

	for( uint i = 0;i < this->Items.size(); i++ )
	{
		if( this->Items[i] == Which )
		{
			this->Items[i]->Cleanup();
			this->Items.erase( this->Items.begin() + i);
			i--;
		}
	}

	this->m_Mutex.Unlock();
}

uint SE_Man_Manager::Gen_ID()
{
	this->m_Mutex.Wait_For_Lock( ); // FILE_LINE.c_str() );
	this->m_Mutex.Lock();

	uint nID = 0;

	for( uint i = 0;i < this->Items.size(); i++ )
		if( this->Items[i]->Handle_ID == nID )
			nID++;// next id

	this->m_Mutex.Unlock();

	return nID;
}

#endif
