#include "Input_Codes.hpp"

#ifndef _WIN32
#include<X11/X.h>
#include<X11/Xlib.h>
#include <X11/keysym.h>
#endif

namespace SE_Input_Codes
{
	// Tables
	
}
using namespace SE_Input;
using namespace SE_Input_Codes;

/*
uchar SE_Input::Incode_To_Ascii( ushort Code )
{
	if( Is_Num( Code ) ) // convert to ascii number
	{

	}
	else if( Is_Alpha( Code ) )
}

ushort SE_Input::Ascii_To_Incode( ushort Code )
{

}
*/

bool SE_Input::Is_Alpha( ushort Code )
{
	return ( Code >= KEY_A ) && ( Code <= KEY_Z );
}

bool SE_Input::Is_Digit( ushort Code )
{
	return ( Code >= KEY_0 ) && ( Code <= KEY_9 );
}

bool SE_Input::Is_Alnum( ushort Code )
{
	return ( Code >= KEY_0 ) && ( Code <= KEY_Z );
}

bool SE_Input::Is_Valid( ushort Code )
{
	return ( Code >= BUTTON_FIRST ) && ( Code <= BUTTON_LAST );
}

bool SE_Input::Is_Keyboard( ushort Code )
{
	return ( Code >= KEY_FIRST ) && ( Code <= KEY_LAST );
}

bool SE_Input::Is_Mouse( ushort Code )
{
	return ( Code >= MOUSE_FIRST ) && ( Code <= MOUSE_LAST );
}




/**************    CONVERSIONS    *********************/

#ifdef _WIN32

/*@@@@ CONVERT VIRTUAL KEYS @@@@@*/
ushort Convert::From_Virtual_Key( uint v_Key )
{
	switch( v_Key )
	{
	case VK_ESCAPE: 		return KEY_ESCAPE;
	case VK_RETURN: 		return KEY_RETURN;
	case VK_SPACE: 			return KEY_SPACE;
	case VK_BACK:			return KEY_BACKSPACE;
	case VK_TAB:			return KEY_TAB;
	case VK_INSERT:			return KEY_INSERT;
	case VK_DELETE: 		return KEY_DELETE;
	case VK_HOME: 			return KEY_HOME;
	case VK_END: 			return KEY_END;
	case VK_PRIOR:	 		return KEY_PAGEUP;
	case VK_NEXT:			return KEY_PAGEDOWN;
	case VK_UP: 			return KEY_UP;
	case VK_LEFT: 			return KEY_LEFT;
	case VK_DOWN: 			return KEY_DOWN;
	case VK_RIGHT: 			return KEY_RIGHT;

// Non ascii characters
	case VK_F1:				return KEY_F1;
	case VK_F2:				return KEY_F2;
	case VK_F3:				return KEY_F3;
	case VK_F4:				return KEY_F4;
	case VK_F5:				return KEY_F5;
	case VK_F6:				return KEY_F6;
	case VK_F7:				return KEY_F7;
	case VK_F8:				return KEY_F8;
	case VK_F9:				return KEY_F9;
	case VK_F10:			return KEY_F10;
	case VK_F11:			return KEY_F11;
	case VK_F12:			return KEY_F12;

/*	case VK_ KEY_CAPSLOCK;
	case VK_ KEY_NUMLOCK;
	case VK_ KEY_SCROLLLOCK;
	case VK_ KEY_CAPSLOCKTOGGLE;
	case VK_ KEY_NUMLOCKTOGGLE;
	case VK_ KEY_SCROLLLOCKTOGGLE;
*/

	case VK_CANCEL:			return KEY_BREAK;
	case VK_LSHIFT:			return KEY_SHIFT_L;
	case VK_RSHIFT:			return KEY_SHIFT_R;
	case VK_MENU:			return KEY_ALT_L;
//	case VK_MENU:			return KEY_ALT_R;
	case VK_LCONTROL:		return KEY_CONTROL_L;
	case VK_RCONTROL:		return KEY_CONTROL_R;
	case VK_LWIN:			return KEY_Super_L;
	case VK_RWIN:			return KEY_Super_R;


// Non alpha-numeric characters
	case VK_OEM_4:			return KEY_LBRACKET;
	case VK_OEM_6:			return KEY_RBRACKET;
	case VK_OEM_1:			return KEY_SEMICOLON;
	case VK_OEM_3:			return KEY_APOSTROPHE;
	case VK_OEM_7:			return KEY_BACKQUOTE;
	case VK_OEM_COMMA:		return KEY_COMMA;
	case VK_OEM_PERIOD:		return KEY_PERIOD;
	case VK_OEM_2:			return KEY_SLASH;
	case VK_OEM_5:			return KEY_BACKSLASH;
	case VK_OEM_MINUS:		return KEY_MINUS;
	case VK_OEM_PLUS:		return KEY_EQUAL;


// Key pad
	case VK_NUMPAD0:			return KEY_PAD_0;
	case VK_NUMPAD1:			return KEY_PAD_1;
	case VK_NUMPAD2:			return KEY_PAD_2;
	case VK_NUMPAD3:			return KEY_PAD_3;
	case VK_NUMPAD4:			return KEY_PAD_4;
	case VK_NUMPAD5:			return KEY_PAD_5;
	case VK_NUMPAD6:			return KEY_PAD_6;
	case VK_NUMPAD7:			return KEY_PAD_7;
	case VK_NUMPAD8:			return KEY_PAD_8;
	case VK_NUMPAD9:			return KEY_PAD_9;
	case VK_DIVIDE:				return KEY_PAD_DIVIDE;
	case VK_MULTIPLY:			return KEY_PAD_MULTIPLY;
	case VK_SUBTRACT:			return KEY_PAD_MINUS;
	case VK_ADD:				return KEY_PAD_PLUS;
//	case VK_KP_Enter:			return KEY_PAD_ENTER;
	case VK_DECIMAL:			return KEY_PAD_DECIMAL;


// Alpha numerical
	case 0x30:				return KEY_0;
	case 0x31:				return KEY_1;
	case 0x32:				return KEY_2;
	case 0x33:				return KEY_3;
	case 0x34:				return KEY_4;
	case 0x35:				return KEY_5;
	case 0x36:				return KEY_6;
	case 0x37:				return KEY_7;
	case 0x38:				return KEY_8;
	case 0x39:				return KEY_9;

	case 0x41:				return KEY_A;
	case 0x42:				return KEY_B;
	case 0x43:				return KEY_C;
	case 0x44:				return KEY_D;
	case 0x45:				return KEY_E;
	case 0x46:				return KEY_F;
	case 0x47:				return KEY_G;
	case 0x48:				return KEY_H;
	case 0x49:				return KEY_I;
	case 0x4A:				return KEY_J;
	case 0x4B:				return KEY_K;
	case 0x4C:				return KEY_L;
	case 0x4D:				return KEY_M;
	case 0x4E:				return KEY_N;
	case 0x4F:				return KEY_O;
	case 0x50:				return KEY_P;
	case 0x51:				return KEY_Q;
	case 0x52:				return KEY_R;
	case 0x53:				return KEY_S;
	case 0x54:				return KEY_T;
	case 0x55:				return KEY_U;
	case 0x56:				return KEY_V;
	case 0x57:				return KEY_W;
	case 0x58:				return KEY_X;
	case 0x59:				return KEY_Y;
	case 0x5A:				return KEY_Z;
	}

	return BUTTON_ERROR;
}

#else
/*@@@@ CONVERT SCAN CODES @@@@@*/
Input_Event Convert::From_Scan_Code( uint v_Key )
{
;//	return Input_Event();
}

/*@@@@ CONVERT KEY SYMBOLS @@@@@*/
#ifndef XK_LATIN1
#define XK_LATIN1 1
#endif
#include <X11/keysymdef.h>

ushort Convert::From_Keysym( uint Keysym )
{
	switch( Keysym )
	{
	case XK_Escape: 		return KEY_ESCAPE;
	case XK_Return: 		return KEY_RETURN;
	case XK_space: 			return KEY_SPACE;
	case XK_BackSpace:		return KEY_BACKSPACE;
	case XK_Tab:			return KEY_TAB;
	case XK_Insert:			return KEY_INSERT;
	case XK_Delete: 		return KEY_DELETE;
	case XK_Home: 			return KEY_HOME;
	case XK_End: 			return KEY_END;
	case XK_Page_Up: 		return KEY_PAGEUP;
	case XK_Page_Down: 		return KEY_PAGEDOWN;
	case XK_Up: 			return KEY_UP;
	case XK_Left: 			return KEY_LEFT;
	case XK_Down: 			return KEY_DOWN;
	case XK_Right: 			return KEY_RIGHT;

// Non ascii characters
	case XK_F1:				return KEY_F1;
	case XK_F2:				return KEY_F2;
	case XK_F3:				return KEY_F3;
	case XK_F4:				return KEY_F4;
	case XK_F5:				return KEY_F5;
	case XK_F6:				return KEY_F6;
	case XK_F7:				return KEY_F7;
	case XK_F8:				return KEY_F8;
	case XK_F9:				return KEY_F9;
	case XK_F10:			return KEY_F10;
	case XK_F11:			return KEY_F11;
	case XK_F12:			return KEY_F12;

/*	case XK_ KEY_CAPSLOCK;
	case XK_ KEY_NUMLOCK;
	case XK_ KEY_SCROLLLOCK;
	case XK_ KEY_CAPSLOCKTOGGLE;
	case XK_ KEY_NUMLOCKTOGGLE;
	case XK_ KEY_SCROLLLOCKTOGGLE;
*/

	case XK_Break:			return KEY_BREAK;
	case XK_Shift_L:		return KEY_SHIFT_L;
	case XK_Shift_R:		return KEY_SHIFT_R;
	case XK_Alt_L:			return KEY_ALT_L;
	case XK_Alt_R:			return KEY_ALT_R;
	case XK_Control_L:		return KEY_CONTROL_L;
	case XK_Control_R:		return KEY_CONTROL_R;
	case XK_Super_L:		return KEY_Super_L;
	case XK_Super_R:		return KEY_Super_R;


// Non alpha-numeric characters
	case XK_bracketleft:	return KEY_LBRACKET;
	case XK_bracketright:	return KEY_RBRACKET;
	case XK_semicolon:		return KEY_SEMICOLON;
	case XK_apostrophe:		return KEY_APOSTROPHE;
	case XK_quoteleft:		return KEY_BACKQUOTE;
	case XK_comma:			return KEY_COMMA;
	case XK_period:			return KEY_PERIOD;
	case XK_slash:			return KEY_SLASH;
	case XK_backslash:		return KEY_BACKSLASH;
	case XK_minus:			return KEY_MINUS;
	case XK_equal:			return KEY_EQUAL;


// Key pad
	case XK_KP_0:			return KEY_PAD_0;
	case XK_KP_1:			return KEY_PAD_1;
	case XK_KP_2:			return KEY_PAD_2;
	case XK_KP_3:			return KEY_PAD_3;
	case XK_KP_4:			return KEY_PAD_4;
	case XK_KP_5:			return KEY_PAD_5;
	case XK_KP_6:			return KEY_PAD_6;
	case XK_KP_7:			return KEY_PAD_7;
	case XK_KP_8:			return KEY_PAD_8;
	case XK_KP_9:			return KEY_PAD_9;
	case XK_KP_Divide:		return KEY_PAD_DIVIDE;
	case XK_KP_Multiply:	return KEY_PAD_MULTIPLY;
	case XK_KP_Subtract:	return KEY_PAD_MINUS;
	case XK_KP_Add:			return KEY_PAD_PLUS;
	case XK_KP_Enter:		return KEY_PAD_ENTER;
	case XK_KP_Decimal:		return KEY_PAD_DECIMAL;


// Alpha numerical
	case XK_0:				return KEY_0;
	case XK_1:				return KEY_1;
	case XK_2:				return KEY_2;
	case XK_3:				return KEY_3;
	case XK_4:				return KEY_4;
	case XK_5:				return KEY_5;
	case XK_6:				return KEY_6;
	case XK_7:				return KEY_7;
	case XK_8:				return KEY_8;
	case XK_9:				return KEY_9;

	case XK_a:				return KEY_A;
	case XK_b:				return KEY_B;
	case XK_c:				return KEY_C;
	case XK_d:				return KEY_D;
	case XK_e:				return KEY_E;
	case XK_f:				return KEY_F;
	case XK_g:				return KEY_G;
	case XK_h:				return KEY_H;
	case XK_i:				return KEY_I;
	case XK_J:				return KEY_J;
	case XK_k:				return KEY_K;
	case XK_l:				return KEY_L;
	case XK_m:				return KEY_M;
	case XK_n:				return KEY_N;
	case XK_o:				return KEY_O;
	case XK_p:				return KEY_P;
	case XK_q:				return KEY_Q;
	case XK_r:				return KEY_R;
	case XK_s:				return KEY_S;
	case XK_t:				return KEY_T;
	case XK_u:				return KEY_U;
	case XK_v:				return KEY_V;
	case XK_w:				return KEY_W;
	case XK_x:				return KEY_X;
	case XK_y:				return KEY_Y;
	case XK_z:				return KEY_Z;
	}

	return BUTTON_ERROR;
}

#endif



/*@@@@ CONVERT ASCII @@@@@*/
Input_Event Convert::From_Ascii( uchar Char )
{
	return Input_Event(NULL, 0, 0);
}

const char* Conv_String_Table[] =
{
		"none",
		"return",
		"space",
		"backspace",
		"escape",
		"tab",
		"insert",
		"delete",
		"home",
		"end",
		"pageup",
		"pagedown",
		"uo",
		"left",
		"down",
		"right",


	// Non ascii characters
		"f1",
		"f2",
		"f3",
		"f4",
		"f5",
		"f6",
		"f7",
		"f8",
		"f9",
		"f10",
		"f11",
		"f12",

		"capslock",
		"numlock",
		"scrolllock",
		"capslock-toggle",
		"numlock-toggle",
		"scrolllock-toggle",

		"break",
		"left-shift",
		"right-shift",
		"left-alt",
		"right-alt",
		"left-control",
		"right-control",
		"left-suoer",
		"right-super",

	// Non alpha-numeric characters
		"[",
		"]",
		";",
		"'",
		"`",
		",",
		".",
		"/",
		"\\",
		"-",
		"=",

	// Key pad
		"kp-0",
		"kp-1",
		"kp-2",
		"kp-3",
		"kp-4",
		"kp-5",
		"kp-6",
		"kp-7",
		"kp-8",
		"kp-9",
		"kp-divide",
		"kp-multiply",
		"kp-minus",
		"kp-plus",
		"kp-enter",
		"kp-decimal",


	// Alpha numerical
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",

		"a",
		"b",
		"c",
		"d",
		"e",
		"f",
		"g",
		"h",
		"i",
		"j",
		"k",
		"l",
		"m",
		"n",
		"o",
		"p",
		"q",
		"r",
		"s",
		"t",
		"u",
		"v",
		"w",
		"x",
		"y",
		"z"
};

ushort Convert::Str_To_ID( const std::string& Name )
{
	for( ushort i = 0; i <= BUTTON_LAST; i++ )
	{
		if( Conv_String_Table[i] == Name )
			return i; // code
	}

	return BUTTON_ERROR;
}

std::string Convert::ID_To_Str( ushort Code )
{
	if( Is_Valid( Code ) )
		return Conv_String_Table[Code];

	return "N/A";
}
