#ifndef ENGINE_DEBUG
#define ENGINE_DEBUG

#include <string>
#include <vector>

#include "../Types.hpp"

namespace SE_Debug
{
	void Watch( void* Data, std::string Where, uint Size );
	void Unwatch( void* Data );

	void Register_Module();

	struct D_Debug;

	struct D_Watch_Variable//watch a variable, if changes record value
	{
		virtual void Watch()=0;
		virtual void Update(D_Debug* Parent)=0;
	};
	struct D_Watch_UInt:public D_Watch_Variable
	{
		void Watch(unsigned int* Var);
		void Update(D_Debug* Parent);
	};


	//
	struct D_Debug
	{
		template <class T>
		bool Add_Watch(T Variable);
		std::vector<D_Watch_Variable*> Watches;
	};
}

struct S_Error
{
	S_Error();
	S_Error(std::string nMessage,unsigned int nLevel);
	std::string Message;
	uint Level;
	/*
		levels:
				0:nothing
				1:make log of
				2:do not continue
				3:shutdown program with message

				4:only warn if error
				5:debug message
	*/
};

#endif








/*
 *  0	Emergency		emerg (panic)	System is unusable.					A "panic" condition usually affecting multiple apps/servers/sites. At this level it would usually notify all tech staff on call.
	1	Alert			alert			Action must be taken immediately.	Should be corrected immediately, therefore notify staff who can fix the problem. An example would be the loss of a primary ISP connection.
	2	Critical		crit			Critical conditions.				Should be corrected immediately, but indicates failure in a secondary system, an example is a loss of a backup ISP connection.
	3	Error			err (error)		Error conditions.					Non-urgent failures, these should be relayed to developers or admins; each item must be resolved within a given time.
	4	Warning			warning (warn)	Warning conditions.					Warning messages, not an error, but indication that an error will occur if action is not taken, e.g. file system 85% full - each item must be resolved within a given time.
	5	Notice			notice			Normal but significant condition.	Events that are unusual but not error conditions - might be summarized in an email to developers or admins to spot potential problems - no immediate action required.
	6	Informational	info			Informational messages.				Normal operational messages - may be harvested for reporting, measuring throughput, etc. - no action required.
	7	Debug			debug			Debug-level messages.				Info useful to developers for debugging the application, not useful during operations.
 */






