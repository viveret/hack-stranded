#ifndef SURVIVOR_ENGINE_INTERACTIVE_TEXT
#define SURVIVOR_ENGINE_INTERACTIVE_TEXT

#include <string>
#include <vector>
#include <map>

namespace SE_Interactive_Text
{
	struct IT_Obj
	{
		virtual void Cleanup();
		IT_Obj* Get_Child( const std::string& Name );
		void* Get_Detail( const std::string& Name );

		std::string Name;
		std::vector< IT_Obj* > Children;
		std::map< std::string, void(*)() > Details;
	};

	void Input_Str( const std::string& Text );
	void Register_Obj( IT_Obj* nObj );
	void Get_Obj( std::string Name );

	const IT_Obj* Get_Global();
/* 
	namespace Actions
	{enum{
		ACT_UNKNWON = 0,

		// Senses
		ACT_LOOK,
		ACT_TOUCH,
		ACT_SMELL,
		ACT_TASTE
	};}
*/
}



#endif
