#include "../../app/Window.hpp"

#include "../../utility/Functions.hpp"
#include "../../math/Math.hpp"
#include "../../module/Module.hpp"
#include "../../module/Events.hpp"
#include "../../module/Event_Defs.hpp"
#include "../../os/Input.hpp"

#include <sstream>

#define Down_Before (lParam & (1 << 30))

#ifdef _WIN32
LRESULT CALLBACK SE_Window::MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:case WM_CLOSE:case WM_QUIT:
		{
			sePrintf( SEPRINT_INFO, "Shutdown by window message: %d\n", uMsg );
			S_Engine::Event_Add_Engine( new SE_Events::Event_Base( NULL, Event_Module::EVENT_SHUTDOWN ) );

			return 0;
		}break;  	  
		case WM_COMMAND:
			{
#ifdef _TEST
				Engine.Wind_Command(LOWORD(wParam));
#endif
			}break;


		case WM_SIZE:case WM_SIZING:
			{	   
				unsigned int Width = LOWORD(lParam);
				unsigned int Height = HIWORD(lParam);

				S_Engine::Event_Add(new SE_Events::Event_Var2<uint,uint>( SE_Modules::Mod_Graphics, Event_Graphic::EVENT_RESIZE, Width, Height ));

#ifdef _TEST
				E_Events::Event_uint2* nEvent = new E_Events::Event_uint2;
				nEvent->Type = E_Events::Event_Types::TYPE_APP;
				nEvent->Code = E_Events::Events::EVENT_WIN_RESIZE;
				nEvent->Argument_1 = Width;
				nEvent->Argument_2 = Height;

				Engine.Event_Manager.Add_Event( nEvent, "WM_SIZE" ); 
#endif
			}break;
		case WM_MOVE://case WM_MOVING:
			{
				unsigned int nX=LOWORD(lParam);
				unsigned int nY=HIWORD(lParam);
#ifdef _TEST
				Engine.Application.Window.Move(nX,nY);
#endif
			}break;


						 
		//mouse input
		case WM_MOUSEWHEEL:
			{
				short Wheel_Dist=GET_WHEEL_DELTA_WPARAM(wParam);

				if( Wheel_Dist < 0 )
					SE_Input::New_Input( SE_Input_Codes::MOUSE_WHEEL_DOWN, 0 );

				else
					SE_Input::New_Input( SE_Input_Codes::MOUSE_WHEEL_UP, 0 );
//				Engine.Application.Input.Mouse_Scroll += Sign(Wheel_Dist);
			}break;
		case WM_MOUSEMOVE:
			{
				SE_Input::Set( SE_Input_Codes::MOUSE_MOVE_X, LOWORD(lParam) );
				SE_Input::Set( SE_Input_Codes::MOUSE_MOVE_Y, HIWORD(lParam) );
//				Engine.Application.Input.Mouse_Move(LOWORD(lParam),HIWORD(lParam));
			}break;	
		case WM_LBUTTONDOWN:
			{
				SE_Input::New_Input( SE_Input_Codes::MOUSE_LEFT, true );
//				if(!Down_Before)Engine.Application.Input.Mouse_L_Down=true;
			}break;
		case WM_LBUTTONUP:
			{
				SE_Input::New_Input( SE_Input_Codes::MOUSE_LEFT, false );
//				if(!Down_Before)Engine.Application.Input.Mouse_L_Down=false;
			}break;
		case WM_RBUTTONDOWN:
			{
				SE_Input::New_Input( SE_Input_Codes::MOUSE_RIGHT, true );
//				if(!Down_Before)Engine.Application.Input.Mouse_R_Down=true;
			}break;
		case WM_RBUTTONUP:
			{
				SE_Input::New_Input( SE_Input_Codes::MOUSE_RIGHT, false );
//				if(!Down_Before)Engine.Application.Input.Mouse_R_Down=false;
			}break;


		case WM_KEYUP:
			{
//				if(!Engine.Application.Input.XBOX.InUse && Down_Before)
				if( Down_Before )
					SE_Input::New_Virtualkey( wParam, false );
//					Engine.Application.Input.Keys->Update(wParam,false);	
			}break;
		case WM_KEYDOWN:
			{
//				if(!Engine.Application.Input.XBOX.InUse && !Down_Before)
				if( !Down_Before )
					SE_Input::New_Virtualkey( wParam, true );
//					Engine.Application.Input.Keys->Update(wParam,true);	   
			}break;

		case WM_GETMINMAXINFO:
			{			   
				( ( MINMAXINFO * )lParam )->ptMinTrackSize.x = 640;  
				( ( MINMAXINFO * )lParam )->ptMinTrackSize.y = 480;  
				return 0;
			}break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

#endif
