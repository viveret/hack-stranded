#ifndef ENGINE_DEBUG
#define ENGINE_DEBUG

#include <string>
#include <vector>

#include "../Types.hpp"

namespace SE_Debug
{
	SE_API void Watch( void* Data, std::string Where, uint Size );
	SE_API void Unwatch( void* Data );

	void Register_Module();

	std::vector< void* > Traceback( ushort Depth = ushort(-1), uchar Skip = 0 );
	std::vector< std::string > Traceback_Str_Vector( ushort Depth = ushort(-1) );
	std::string Traceback_Str( ushort Depth = ushort(-1) );

	std::string Function_Demangle( const char* Func );

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
	SE_API S_Error();
	SE_API S_Error(std::string nMessage,unsigned int nLevel);
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
