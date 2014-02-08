#ifndef SURVIVOR_ENGINE_APPLICATION
#define SURVIVOR_ENGINE_APPLICATION


#include "../Core.hpp"
#include "../module/Events.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include "../pugixml/pugixml.hpp"

#include "../debug/Debug.hpp"
#include "../app/XBox.hpp"
#include "../math/Math.hpp"
#include "../utility/Array.hpp"
#include "../gl/OpenGL.hpp"
#include "../debug/Log.hpp"

#ifdef _WIN32
#define SE_APP_WIN_CLASS "Survivor_Engine_Win_Class"
#endif

namespace SE_Application//engine application
{
	typedef pugi::xml_document XML_File;
	typedef pugi::xml_attribute XML_Attr;
	typedef pugi::xml_node XML_Node;


	// AP = APPLICATION PLATFORM
#ifdef _TEST
	struct AP_IOAPI
	{
		AP_IOAPI();

		// FILE SYSTEM AND OTHER LOW LEVEL
		std::string Load_Zip_File_Str( std::string File, bool Silent = false );//load raw file, can be silent = no log
		std::string Load_File_Str( std::string File, bool Silent = false );//load raw file, can be silent = no log

		// Platform specific
		virtual SE_Raw_Buffer Load_Zip_File( std::string File, bool Silent = false );//load raw file, can be silent = no log
		virtual SE_Raw_Buffer Load_File( std::string File, bool Silent = false );//load raw file, can be silent = no log

		bool Load_XML_File( XML_File* File, std::string Dest );


		// CONSTANT AND COMMON FRQUENCY USE
		bool Typing, Uppercase;//is typing
		std::string Typing_What;

		void (*Submit_String)(std::string);
		void (*String_Keypress)( uchar );
		void Finished_Typing();

		XBOXController XBOX;

		void Get_Mouse();
		void Set_Mouse( int x, int y );

		int Mouse_X, Mouse_Y;
		int Mouse_Scroll;
		bool Mouse_L_Down, Mouse_R_Down;//menu
	};
	void String_Submit_Default( std::string Str );// default
	void String_Keypress_All( uchar l );
#endif
	void Set_OpenGL_Settings();
	void Switch_To_2D();
	void Switch_To_3D();
}

#endif
