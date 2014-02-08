#include "Events.hpp"
using namespace SE_Events;
#include "../os/Threader.hpp"

Event_Base::Event_Base( SE_Module_ID nType, ushort nCode )
{
	this->m_Type = nType;
	this->Code = nCode;
}

Event_Base* Event_Base::Clone()
{
	Event_Base* nEv = new Event_Base( this->m_Type, this->Code );

	*nEv = *this;

	return nEv;
}



Event_Manager_Base::Event_Manager_Base()
{
	
}

Event_Manager_Base::~Event_Manager_Base()
{
	this->Cleanup();
}

void Event_Manager_Base::Cleanup()
{
	this->m_Mutex.Wait_For_Lock( );// __FUNCTION__ );
	this->m_Mutex.Lock();

	while( this->Event_List.size() > 0 )
		this->Pop_Event();

	this->m_Mutex.Unlock();
}				
							 
void Event_Manager_Base::Update()
{
	uint Num_Skipped = 0;
	while( this->Event_List.size() > Num_Skipped )
	{
		if( this->Handle_Event( this->Event_List.front() ) ) // OK to delete
			this->Pop_Event();
		else
			Num_Skipped++;
	}
}

void Event_Manager_Base::Add_Event( Event_Base* Event )
{
	this->m_Mutex.Wait_For_Lock( );// __FUNCTION__ );
	this->m_Mutex.Lock();

	this->Event_List.push( Event );	  

	this->m_Mutex.Unlock();

#ifdef _TEST
	Engine.Mem_Add( Event, "event" );
#endif
	//Engine.Application.Event_Log.Write_Log("Event Added: "+Event->To_String()); 
}

void Event_Manager_Base::Pop_Event()
{
	if( this->Event_List.size() == 0 )
		return;		

	//Engine.Application.Event_Log.Write_Log("Event Removed: "+this->Event_List.front()->To_String() + "...");	   
	this->m_Mutex.Wait_For_Lock( ); // __FUNCTION__ );
	this->m_Mutex.Lock();
#ifdef _TEST
	Engine.Mem_Remove( this->Event_List.front() );
#endif
	delete this->Event_List.front();
	this->Event_List.pop();																				   

	//Engine.Application.Event_Log.Write("Success");	   

	this->m_Mutex.Unlock();
}








Event_Manager::Event_Manager()
{
	this->Processor = NULL;
}

bool Event_Manager::Handle_Event( Event_Base* Event )
{
	if( this->Processor == NULL )
		return true;

	return this->Processor( Event );
}
