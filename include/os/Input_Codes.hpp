#ifndef SURVIVOR_ENGINE_INPUT_CODES
#define SURVIVOR_ENGINE_INPUT_CODES

#include "../module/Events.hpp"

namespace SE_Input_Codes
{enum {
	BUTTON_ERROR = -1,
	BUTTON_UNKNOWN = 0,
	BUTTON_NONE =	BUTTON_UNKNOWN,
	BUTTON_FIRST = BUTTON_NONE + 1,


	KEY_FIRST = BUTTON_FIRST,
// Action keys
	KEY_ENTER = KEY_FIRST,
	KEY_RETURN = KEY_ENTER,
	KEY_BACKSPACE,
	KEY_ESCAPE,
	KEY_TAB,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,


// Non ascii characters
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,

	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_SCROLLLOCK,
	KEY_CAPSLOCKTOGGLE,
	KEY_NUMLOCKTOGGLE,
	KEY_SCROLLLOCKTOGGLE,

	KEY_BREAK,
	KEY_SHIFT_L,
	KEY_SHIFT_R,
	KEY_ALT_L,
	KEY_ALT_R,
	KEY_CONTROL_L,
	KEY_CONTROL_R,
	KEY_Super_L,
	KEY_Super_R,


// Non alpha-numeric characters, still ASCII

	KEY_ASCII_START,

	KEY_LBRACKET = KEY_ASCII_START,
	KEY_RBRACKET,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_BACKQUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_BACKSLASH,
	KEY_MINUS,
	KEY_EQUAL,

	KEY_SPACE,

// Key pad
	KEY_PAD_0,
	KEY_PAD_1,
	KEY_PAD_2,
	KEY_PAD_3,
	KEY_PAD_4,
	KEY_PAD_5,
	KEY_PAD_6,
	KEY_PAD_7,
	KEY_PAD_8,
	KEY_PAD_9,
	KEY_PAD_DIVIDE,
	KEY_PAD_MULTIPLY,
	KEY_PAD_MINUS,
	KEY_PAD_PLUS,
	KEY_PAD_ENTER,
	KEY_PAD_DECIMAL,


// Alpha numerical
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,

	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,


	KEY_LAST = KEY_Z,
//	KEY_COUNT = KEY_LAST - KEY_FIRST + 1,



// Mouse
	MOUSE_FIRST = KEY_LAST + 1,

	MOUSE_LEFT = MOUSE_FIRST,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_WHEEL_UP,		// 'pressed' and 'released' when the wheel is moved up
	MOUSE_WHEEL_DOWN,	// 'pressed' and 'released' when the wheel is moved down
	MOUSE_WHEEL_POS,
	MOUSE_MOVE_X, MOUSE_X = MOUSE_MOVE_X,
	MOUSE_MOVE_Y, MOUSE_Y = MOUSE_MOVE_Y,

	MOUSE_LAST = MOUSE_MOVE_Y,

	BUTTON_LAST = MOUSE_LAST
};}

namespace SE_Input
{
	typedef SE_Events::Event_Var<sshort> Input_Event;

	bool Is_Alpha( ushort Code );
	bool Is_Digit( ushort Code );
	bool Is_Alnum( ushort Code );

	bool Is_Valid( ushort Code );

	bool Is_Keyboard( ushort Code );
	bool Is_Mouse( ushort Code );

	namespace Convert
	{
#ifdef _WIN32
		ushort From_Virtual_Key( uint v_Key );
#else
		Input_Event From_Scan_Code( uint v_Key );
		ushort From_Keysym( long unsigned int Keysym );
#endif
		Input_Event From_Ascii( uchar Char );
		uchar To_ASCII( Input_Event* nEv );

		std::string ID_To_Str( ushort Code );
		ushort Str_To_ID( const std::string& Name );

	}

}


#endif
