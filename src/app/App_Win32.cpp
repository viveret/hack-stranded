#ifdef _WIN32_NOPE

#include "App_Win32.hpp"

using namespace SE_App_Win32;

A_Win32_Window::A_Win32_Window()
{
	this->hEng_RC=NULL;//  rendering context, links opengl calls to engine
	this->hDraw_RC=NULL;// and draw threads

	this->hDC=NULL;//    device context         draw to the window
	this->hWnd=NULL;//  window handle
	this->hInstance=NULL;//               instance of application
}



#ifdef _WIN32
void A_Window::Create_Window(HINSTANCE nhInstance, std::string nName, std::string nClass_Name )
{
	this->Class_Name = nClass_Name;

	InitCommonControls();

	this->hInstance=nhInstance;

	this->Wind_w = GetSystemMetrics(SM_CXSCREEN);
	this->Wind_h = GetSystemMetrics(SM_CYSCREEN);

	GLuint		PixelFormat;			// Holds Results
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;

	WindowRect.left = (long)0;
	WindowRect.right = (long)this->Wind_w;
	WindowRect.top = (long)0;
	WindowRect.bottom = (long)this->Wind_h;

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) MainWindowProc;
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= this->hInstance;
	wc.hIcon			= LoadIcon(this->hInstance,MAKEINTRESOURCE(WINDOW_ICON));
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= this->Class_Name.c_str();

	if (!RegisterClass(&wc))
	{
		Engine.Win32_Assert( "Failed To Register The Window Class", 3 );
		return;
	}

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    dwStyle = WS_OVERLAPPEDWINDOW | WS_THICKFRAME;

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);



    // Create The Window
	if (!(this->hWnd=CreateWindowEx(dwExStyle,
								this->Class_Name.c_str(),							// Class Name
								this->Name.c_str(),								// Window Title
								dwStyle |
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								CW_USEDEFAULT, CW_USEDEFAULT,
								WindowRect.right-WindowRect.left,
								WindowRect.bottom-WindowRect.top,
								NULL,								// No Parent Window
								NULL,								// No Menu
								this->hInstance,
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		Engine.Win32_Assert( "Window Creation Error", 3 );
		return;
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

	if (!(this->hDC=GetDC(this->hWnd)))// Did We Get A Device Context?
	{
		Engine.Win32_Assert( "Can't Create A GL Device Context", 3 );return;
	}

	if (!(PixelFormat=ChoosePixelFormat(this->hDC,&pfd)))// Did Windows Find A Matching Pixel Format?
	{
		Engine.Win32_Assert( "Can't Find A Suitable PixelFormat", 3 );return;
	}

	if(!SetPixelFormat(this->hDC,PixelFormat,&pfd))// Are We Able To Set The Pixel Format?
	{
		Engine.Win32_Assert( "Can't Set The PixelFormat", 3 );return;
	}

	if (!(this->hEng_RC=wglCreateContext(this->hDC)))// Are We Able To Get A Rendering Context?
	{
		Engine.Win32_Assert( "Can't Create A GL Rendering Context For The Engine", 3 );return;
	}

	if (!(this->hDraw_RC=wglCreateContext(this->hDC)))// Are We Able To Get A Rendering Context?
	{
		Engine.Win32_Assert( "Can't Create A GL Rendering Context For The Draw Thread", 3 );return;
	}


	//time to share
	BOOL Share_Error = wglShareLists(this->hEng_RC,this->hDraw_RC);
	if(Share_Error==FALSE)
	{
		Engine.Win32_Assert( "Can't Share Engine and Draw Thread Lists", 3 );return;
	}

	if(!wglMakeCurrent(this->hDC,this->hDraw_RC))// Try To Activate The Rendering Context
	{
		Engine.Win32_Assert( "Can't Activate The GL Rendering Context For Engine", 3 );return;
	}

	ShowWindow(this->hWnd,SW_SHOWMAXIMIZED);						// Show The Window
	SetForegroundWindow(this->hWnd);						// Slightly Higher Priority
	SetFocus(this->hWnd);									// Sets Keyboard Focus To The Window

	Engine.Log( "Initialized Win32 Window\n", Log_Type::TYPE_DBUG );
}


#endif

void A_Window::Destory_Window()
{
	/*
	if(this->Is_Active==false) return;//already nonactive

	this->Is_Active=false; */

#ifdef _WIN32
	if(this->hEng_RC || this->hDraw_RC)
	{
		/*if (!wglMakeCurrent(NULL,NULL))//Able to release contexts?
		{
			Engine.Win32_Assert( "Release Of DC And RC Failed", 3 );return;
		} */

		if (!wglDeleteContext(this->hEng_RC))//Able to delete?
		{
			Engine.Win32_Assert( "Release Rendering Context For Engine Failed", 3 );return;
		}
		this->hEng_RC=NULL;

		if (!wglDeleteContext(this->hDraw_RC))//Able to delete?
		{
			Engine.Win32_Assert( "Release Rendering Context For Drawing Failed", 3);return;
		}
		this->hDraw_RC=NULL;
	}

	if (this->hDC && !ReleaseDC(this->hWnd,this->hDC))//Release DC?
	{
		this->hDC=NULL;
		Engine.Win32_Assert( "Release Device Context Failed", 3);return;
	}

	if (this->hWnd && !DestroyWindow(this->hWnd))//Destroy window?
	{
		this->hWnd=NULL;
		Engine.Win32_Assert( "Could Not Release hWnd", 3 );return;
	}

	if (!UnregisterClass(this->Class_Name.c_str(),this->hInstance))//Unregister class?
	{
		this->hInstance=NULL;
		Engine.Win32_Assert( "Could Not Unregister Class", 3 );return;
	}


	PostQuitMessage(0);
#endif
	//finished all the way
}




#endif // win32
