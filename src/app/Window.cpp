#include "Window.hpp"
#include "../Core.hpp"
#include "../module/Module.hpp"
#include "../module/Events.hpp"
#include "../module/Event_Defs.hpp"
#include "../os/Input.hpp"

#ifdef _WIN32

#include <CommCtrl.h>
#include "../os/win32/Icons.hpp"

#define SE_WIN32_WIN_CLASS "Survivor_Engine_Win_Class"

#else

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif

#ifdef _WIN32
#include <GL/GL.h>
#endif

#include <memory.h>

namespace SE_Modules
{
SE_Module_ID Mod_Window = NULL;
}
using namespace SE_Modules;

namespace SE_Window
{
	uint Create_Window( const std::vector<std::string>& mArgs );
	uint Create_GL_Context( const std::vector<std::string>& mArgs );
	uint Destroy_Window();
	uint Destroy_GL_Context();

	bool Event_Thread( SE_Events::Event_Base* nEv );

/***********    GLOBALS    ************/
	uint Wind_w = 640, Wind_h = 480;
	int Wind_x = 0, Wind_y = 0;
	uchar Border_W = 0, Border_H = 0;

	bool Fullscreen = false, Focus = false;
	std::string Name = "Survivor Engine Unnamed Window";

	bool Is_Active = false;

#ifdef _WIN32
	HGLRC hEng_RC = NULL, hDraw_RC = NULL;//  rendering context, links opengl calls to engine and draw threads
	HDC hDC = NULL;//    device context         draw to the window
	HWND hWnd = NULL;//  window handle
	HINSTANCE hInstance = NULL;//               instance of application
#else
	Display                 *m_dpy = NULL;
	Window                  m_root;
	XVisualInfo             *m_vi = NULL;
	Colormap                m_cmap;
	XSetWindowAttributes    m_swa;
	Window                  m_win;
	GLXContext              m_glc;
	XWindowAttributes       m_gwa;
	XEvent                  m_xev;

	// OTHER INFO
	Atom wmDeleteMessage;

	bool Called_GLX_Make_Cur = false;
#endif
}
using namespace SE_Window;



/*****************   WINDOW OPERATIONS   *****************/

uint SE_Window::Create_Window( const std::vector<std::string>& mArgs )
{
	S_Engine::Module_Set_Color( "\x1b[34m" );

#ifdef _WIN32
	InitCommonControls();

	hInstance = GetModuleHandle( NULL );

	Wind_w = GetSystemMetrics(SM_CXSCREEN);
	Wind_h = GetSystemMetrics(SM_CYSCREEN);

	GLuint		PixelFormat;			// Holds Results
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;

	WindowRect.left = (long)0;
	WindowRect.right = (long)Wind_w;
	WindowRect.top = (long)0;
	WindowRect.bottom = (long)Wind_h;

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) MainWindowProc;
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(hInstance,MAKEINTRESOURCE(WINDOW_ICON));
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= SE_WIN32_WIN_CLASS;

	if (!RegisterClass(&wc))
	{
		Win32_Assert( "Failed To Register The Window Class", 3 );
		return SE_FAILURE;
	}

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    dwStyle = WS_OVERLAPPEDWINDOW | WS_THICKFRAME;

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);



    // Create The Window
	if (!(hWnd=CreateWindowEx(dwExStyle,
								SE_WIN32_WIN_CLASS,							// Class Name
								Name.c_str(),								// Window Title
								dwStyle |
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								CW_USEDEFAULT, CW_USEDEFAULT,
								WindowRect.right-WindowRect.left,
								WindowRect.bottom-WindowRect.top,
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		Win32_Assert( "Window Creation Error", 3 );
		return SE_FAILURE;
	}

	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		0,											// 0Bit Z-Buffer (Depth Buffer)
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC=GetDC(hWnd)))// Did We Get A Device Context?
	{
		Win32_Assert( "Can't Create A GL Device Context", 3 );return SE_FAILURE;
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))// Did Windows Find A Matching Pixel Format?
	{
		Win32_Assert( "Can't Find A Suitable PixelFormat", 3 );return SE_FAILURE;
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))// Are We Able To Set The Pixel Format?
	{
		Win32_Assert( "Can't Set The PixelFormat", 3 );return SE_FAILURE;
	}

/*	if (!(hEng_RC=wglCreateContext(hDC)))// Are We Able To Get A Rendering Context?
	{
		Win32_Assert( "Can't Create A GL Rendering Context For The Engine", 3 );return false;
	}
	*/


	//time to share
	/*BOOL Share_Error = wglShareLists(hEng_RC,hDraw_RC);
	if(Share_Error==FALSE)
	{
		Win32_Assert( "Can't Share Engine and Draw Thread Lists", 3 );return false;
	}
	*/



	ShowWindow(hWnd,SW_SHOWMAXIMIZED);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);
#else
//	XInitThreads();

	GLint m_att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

	m_dpy = XOpenDisplay(NULL);

	if( m_dpy == NULL )
	{
	    sePrintf( SEPRINT_ERROR, "Cannot Connect To X Server\n" );
	    return SE_FAILURE;
	}
	XLockDisplay( m_dpy );

	m_root = DefaultRootWindow( m_dpy );

	m_vi = glXChooseVisual( m_dpy, 0, m_att );

	if( m_vi == NULL )
	{
	    sePrintf( SEPRINT_ERROR, "No Appropriate Visual Found\n" );
	    return SE_FAILURE;
	}
	else
	{
	    sePrintf( SEPRINT_MODULE, "Visual %p Selected\n", (void *)m_vi->visualid); /* %p creates hexadecimal output like in glxinfo */
	}


	m_cmap = XCreateColormap( m_dpy, m_root, m_vi->visual, AllocNone);


	m_swa.colormap = m_cmap;
	m_swa.event_mask = PointerMotionMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask |
							 StructureNotifyMask | EnterWindowMask | LeaveWindowMask;

	m_win = XCreateWindow( m_dpy, m_root, Wind_x, Wind_y, Wind_w, Wind_h, 0, m_vi->depth,
	                            InputOutput, m_vi->visual, CWColormap | CWEventMask, &m_swa);

	sePrintf( SEPRINT_MODULE, "Created window, x: %d, y: %d, w: %d, h: %d\n", Wind_x, Wind_y, Wind_w, Wind_h );


	wmDeleteMessage = XInternAtom(m_dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols( m_dpy, m_win, &wmDeleteMessage, 1);


	XMapWindow( m_dpy, m_win );
	XStoreName( m_dpy, m_win, Name.c_str() );

	//XSelectInput( m_dpy, m_win, X_Event_List );
//	Focus( true );


	XMapRaised( m_dpy, m_win);

	XEvent xev;
	Atom wm_state  =  XInternAtom(m_dpy, "_NET_WM_STATE", False);
	Atom max_horz  =  XInternAtom(m_dpy, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
	Atom max_vert  =  XInternAtom(m_dpy, "_NET_WM_STATE_MAXIMIZED_VERT", False);

	memset(&xev, 0, sizeof(xev));
	xev.type = ClientMessage;
	xev.xclient.window = m_win;
	xev.xclient.message_type = wm_state;
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = XInternAtom(m_dpy, "_NET_WM_STATE_ABOVE", False);
	xev.xclient.data.l[1] = 1;//_NET_WM_STATE_ADD;
	xev.xclient.data.l[2] = max_horz;
	xev.xclient.data.l[3] = max_vert;

	XSendEvent(m_dpy, DefaultRootWindow(m_dpy), False, SubstructureNotifyMask, &xev);

#endif


/*	if( Application.OpenGL.Initialize() == false )//failed to initialize
	{
		Register_Error(S_Error("Failed to initialize OpenGL interface", 3));
		return false;
	}
*/

	//Reshape(Wind_w,Wind_h);
	//made it here so far, no errors

#ifndef _WIN32
	XGetWindowAttributes( m_dpy, m_win, &m_gwa );
	Reshape( m_gwa.width, m_gwa.height );

	XUnlockDisplay( m_dpy );
#endif


	return SE_SUCCESS;
}

uint SE_Window::Create_GL_Context( const std::vector<std::string>& mArgs )
{
#ifdef _WIN32
	
	if( !(hDraw_RC=wglCreateContext(hDC)) )// Are We Able To Get A Rendering Context?
	{
		Win32_Assert( "Can't Create A GL Rendering Context For The Draw Thread", 3 );return false;
	}
	
	if( !wglMakeCurrent(hDC,hDraw_RC) )// Try To Activate The Rendering Context
	{
		Win32_Assert( "Can't Activate The GL Rendering Context For Engine", 3 );return false;
	}

#else

	m_glc = glXCreateContext( m_dpy, m_vi, NULL, GL_TRUE );
	glXMakeCurrent( m_dpy, m_win, m_glc );
#endif

	return SE_SUCCESS;
}

uint SE_Window::Destroy_Window()
{
#ifdef _WIN32
	if( hEng_RC || hDraw_RC)
	{
		/*if (!wglMakeCurrent(NULL,NULL))//Able to release contexts?
		{
			Win32_Assert( "Release Of DC And RC Failed", 3 );return;
		} */

		if( !wglDeleteContext( hEng_RC ) )//Able to delete?
		{
			Win32_Assert( "Release Rendering Context For Engine Failed", 3 );return SE_FAILURE;
		}
		hEng_RC = NULL;

		if (!wglDeleteContext( hDraw_RC ))//Able to delete?
		{
			Win32_Assert( "Release Rendering Context For Drawing Failed", 3);return SE_FAILURE;
		}
		hDraw_RC=NULL;
	}

	if ( hDC && !ReleaseDC( hWnd, hDC ))//Release DC?
	{
		hDC=NULL;
		Win32_Assert( "Release Device Context Failed", 3);return SE_FAILURE;
	}

	if (hWnd && !DestroyWindow(hWnd))//Destroy window?
	{
		hWnd=NULL;
		Win32_Assert( "Could Not Release hWnd", 3 );return SE_FAILURE;
	}

	if (!UnregisterClass( SE_WIN32_WIN_CLASS, hInstance ))//Unregister class?
	{
		hInstance=NULL;
		Win32_Assert( "Could Not Unregister Class", 3 );return SE_FAILURE;
	}

	PostQuitMessage(0);
#else

	/*XKeyboardControl KB_Control;
	KB_Control.key_click_percent = -1;
	KB_Control.bell_percent = -1;
	KB_Control.bell_pitch = -1;
	KB_Control.bell_duration = -1;
	KB_Control.led = -1;
	KB_Control.led_mode = -1;
	KB_Control.key = -1;
	KB_Control.auto_repeat_mode = 1;

	XChangeKeyboardControl( m_dpy, KBAutoRepeatMode, &KB_Control );*/
	//XAutoRepeatOn( m_dpy );
#ifdef _TEST
	Focus( false );
#endif
	//XCloseDisplay( m_dpy );
    //XDestroyWindow( m_dpy, m_win );
#endif
	return SE_SUCCESS;
}

uint SE_Window::Destroy_GL_Context()
{
#ifdef _WIN32

	wglDeleteContext( hEng_RC );
#else

	XLockDisplay( m_dpy );

    glXMakeCurrent( m_dpy, None, NULL );
   	glXDestroyContext( m_dpy, m_glc );

	XUnlockDisplay( m_dpy );
#endif

	return true;
}

#ifdef _WIN32

void Check_Win32_Events()
{
    MSG msg;
	while( PeekMessage( &msg, hWnd, 0, 0, PM_REMOVE ) > 0 )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

#else

void Check_XLib_Events()
{
	XLockDisplay( m_dpy );
	while( XPending( m_dpy ) != 0 )
	{
		XNextEvent( m_dpy, &m_xev);

		switch( m_xev.type )
		{
		case MotionNotify:
		{
			SE_Input::Set( SE_Input_Codes::MOUSE_MOVE_X, m_xev.xmotion.x );
			SE_Input::Set( SE_Input_Codes::MOUSE_MOVE_Y, m_xev.xmotion.y );
		}break;

		case KeyPress:
		{
			SE_Input::New_Keysym( XLookupKeysym(&m_xev.xkey, 0), true );
		}break;

		case KeyRelease:
		{
			SE_Input::New_Keysym( XLookupKeysym(&m_xev.xkey, 0), false );
		}break;

		case ButtonPress:
		{
			switch( m_xev.xbutton.button )
			{
			case Button1:
				SE_Input::New_Input( SE_Input_Codes::MOUSE_LEFT, true );
			break;
			case Button3:
				SE_Input::New_Input( SE_Input_Codes::MOUSE_RIGHT, true );
			break;
			case Button4:// middle mouse wheel moved
				SE_Input::New_Input( SE_Input_Codes::MOUSE_WHEEL_UP, 0 );
			break;
			case Button5:// middle mouse wheel moved
				SE_Input::New_Input( SE_Input_Codes::MOUSE_WHEEL_DOWN, 0 );
			break;
			}
		}break;

		case ButtonRelease:
		{
			switch( m_xev.xbutton.button )
			{
			case Button1:
				SE_Input::New_Input( SE_Input_Codes::MOUSE_LEFT, false );
			break;
			case Button3:
				SE_Input::New_Input( SE_Input_Codes::MOUSE_RIGHT, false );
			break;
			}
		}break;

		case EnterNotify:
		{
//			Focus( true );
		}break;

		case LeaveNotify:
		{
//			Focus( false );
		}break;

		case ConfigureNotify:
		{
			//XWindowAttributes xwa;
			//XGetWindowAttributes(m_dpy, m_win, &xwa);
			if( Border_H == 0 && Border_W == 0 )// not set
			{
				int x, y;
				Window child_return;

				XTranslateCoordinates(m_dpy, m_win, m_root,
												   0, 0, & x, & y, & child_return);

				Border_W = x - 1;// ionclude pixel border
				Border_H = y - 1;
				XMapRaised( m_dpy, m_win);
			}

			Wind_x = m_xev.xconfigure.x - Border_W;
			Wind_y = m_xev.xconfigure.y - Border_H;

			if( m_xev.xconfigure.width == 0 )	m_xev.xconfigure.width = 1;
			if( m_xev.xconfigure.height == 0 )	m_xev.xconfigure.height = 1;

			Wind_w = m_xev.xconfigure.width;
			Wind_h = m_xev.xconfigure.height;

			S_Engine::Event_Add(new SE_Events::Event_Var2<uint,uint>( SE_Modules::Mod_Graphics, Event_Graphic::EVENT_RESIZE, Wind_w, Wind_h ));
		}break;


		case ClientMessage:
		{
			if( (Atom)m_xev.xclient.data.l[0] == wmDeleteMessage )
			{
				sePrintf( SEPRINT_INFO, "Shutdown by window message\n" );
				S_Engine::Event_Add_Engine( new SE_Events::Event_Base( NULL, Event_Module::EVENT_SHUTDOWN ) );
			}
		}break;
		}
	}
	XUnlockDisplay( m_dpy );
}
#endif

bool SE_Window::Event_Thread( SE_Events::Event_Base* nEv )
{
	if( nEv->m_Type == SE_Modules::Mod_Graphics )
	{
		switch( nEv->Code )
		{
		case Event_Graphic::EVENT_RESIZE:
			{
				Reshape( ((SE_Events::Event_Var2<uint,uint>*)nEv)->Argument1, ((SE_Events::Event_Var2<uint,uint>*)nEv)->Argument2 );
			}break;
		};
	}

	return true;
}

void SE_Window::Reshape( uint w, uint h )
{
	if( SE_Thread::Get_Current() != seThread_Render() )// wrong thread buddy
	{			
		S_Engine::Event_Add_Render( new SE_Events::Event_Var2<uint,uint>(
					SE_Modules::Mod_Graphics, Event_Graphic::EVENT_RESIZE, w, h
					) );
	}
	else
	{
		if( w == 0 )	w = 1;
		if( h == 0 )	h = 1;

		Wind_w = w;
		Wind_h = h;

		glViewport( 0, 0, Wind_w, Wind_h );
	}
}

void SE_Window::Move( uint x, uint y )
{
	Wind_x = x;
	Wind_y = y;
}

uint SE_Window::Get_W()
{
	return Wind_w;
}

uint SE_Window::Get_H()
{
	return Wind_h;
}

void SE_Window::Set_Name( std::string nName )
{
	Name = nName;
#ifdef _WIN32
	if( hWnd )
		SetWindowText( hWnd, nName.c_str() );
#else
	if( m_dpy )
		XStoreName( m_dpy, m_win, Name.c_str() );
#endif
}

#ifdef _WIN32
HWND SE_Window::Get_HWND()
{
	return hWnd;
}
#endif

void SE_Window::Frame_Start()
{
#ifndef _WIN32
	XLockDisplay( m_dpy );
#endif
}

void SE_Window::Frame_End()
{
//	glFinish();
#ifdef _WIN32
	SwapBuffers( hDC );
#else
    glXSwapBuffers( m_dpy, m_win);
	XUnlockDisplay( m_dpy );
#endif

}

#ifdef _WIN32
#include <sstream>
bool SE_Window::Win32_Assert( std::string Info, uint Level )
{
	DWORD n_Error = GetLastError();
	LPSTR lpMsgBuf = NULL;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				  FORMAT_MESSAGE_FROM_SYSTEM |
				  FORMAT_MESSAGE_IGNORE_INSERTS,
				  NULL,n_Error,
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				  (LPTSTR)&lpMsgBuf,0,NULL);

	std::stringstream Str_Err;
	Str_Err << n_Error << '-' << lpMsgBuf;

	Info = "Win32 " + Info + ": " + Str_Err.str();

	LocalFree( lpMsgBuf );

	if( n_Error == ERROR_SUCCESS )
		return false;

//	Register_Error(S_Error( Info, Level ));
	sePrintf( SEPRINT_ERROR, Info.c_str() );

	return true;
}

#endif

/*****************   MODULE OPERATIONS    *********/
#ifdef _WIN32
#define WIN_MOD_NAME "window-Win32"
#else
#define WIN_MOD_NAME "window-XServer"
#endif

void SE_Window::Register_Module()
{
	Mod_Window = S_Engine::Generate_Module( WIN_MOD_NAME );
	S_Engine::Register_Module_Event_Render( Mod_Window, SE_Window::Event_Thread );

	S_Engine::Register_Module_Engine( Mod_Window, S_Engine::Reg_Mod_Time::LOW_LEVEL + 1, Create_Window, Destroy_Window );
	S_Engine::Register_Module_Render( Mod_Window, S_Engine::Reg_Mod_Time::LOW_LEVEL + 1, Create_GL_Context, Destroy_GL_Context );

#ifdef _WIN32
	S_Engine::Register_Module_Thread_Engine( Check_Win32_Events );
#else
	S_Engine::Register_Module_Thread_Engine( Check_XLib_Events );
#endif

	// For drawing properly
	S_Engine::Register_Module_Thread_Render( Frame_Start, S_Engine::Module_Positions::POS_START, 255 );
	S_Engine::Register_Module_Thread_Render( Frame_End, S_Engine::Module_Positions::POS_END, 255 );
}
