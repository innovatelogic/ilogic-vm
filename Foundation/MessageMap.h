#ifndef __MessageMap_h__
#define __MessageMap_h__

#ifdef _WIN32
#pragma once
#endif

#include <vector>

//---------------------------------------------------------------------------------------------
#define VAR_NAME(Var) #Var
#define VAR_NAME2(Var1, Var2) VAR_NAME(Var1)##";"##VAR_NAME(Var2)



//---------------------------------------------------------------------------------------------
enum EActorState
{
	ActorState_None = 0,
	ActorState_Locked,
};

//---------------------------------------------------------------------------------------------
enum EActorStates
{
	AS_NONE								= 0x00000000,
	AS_ENABLED							= 0x00000001,
	AS_VISIBLE							= 0x00000010,
	AS_FOCUSED							= 0x00000100,
	AS_LOCKED							= 0x00001000,
	AS_PRESSED							= 0x00010000,
	AS_MOUSE_OVER						= 0x00100000,
};

//---------------------------------------------------------------------------------------------
enum EActiveState
{
	ActiveState_Normal		= 0,
	ActiveState_Active		= 1,
	ActiveState_Focused		= 2,
	ActiveState_Pressed		= 3,
	ActiveState_Disabled	= 4,
	ActiveState_MAX			= 5, // endl. used for init
};

//---------------------------------------------------------------------------------------------
enum Event
{
	MOUSE_Pressed = 0,
	MOUSE_DoubleClick,
	MOUSE_Released,
	MOUSE_Over,
	MOUSE_Move,
	EVENT_Tick,
	EVENT_Redirect,
	MOUSE_None,
};

//---------------------------------------------------------------------------------------------
enum MouseCode
{
	MOUSE_LEFT = 0,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_4,
	MOUSE_5,
	MOUSE_LAST,
};
//---------------------------------------------------------------------------------------------
enum KeyState
{
	KEY_Pressed = 0,
	KEY_Repeat,
	KEY_Up,
};

//-----------------------------------------------------------------------------
// Purpose: this is a map for virtual key codes
//			virtual key codes may exist outside this range for other languages
//-----------------------------------------------------------------------------
enum KeyCode
{
	KEY_NONE = 0,
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
	KEY_LBRACKET,
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
	KEY_ENTER,
	KEY_SPACE,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_ESCAPE,
	KEY_SCROLLLOCK,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_BREAK,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LALT,
	KEY_RALT,
	KEY_LCONTROL,
	KEY_RCONTROL,
	KEY_LWIN,
	KEY_RWIN,
	KEY_APP,
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,
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
	KEY_CAPSLOCKTOGGLE,
	KEY_NUMLOCKTOGGLE,
	KEY_SCROLLLOCKTOGGLE,
	KEY_LAST,
};

/** key input event data */
struct EventInput
{
	KeyCode  Key;
	KeyState State;
	bool	 ShiftPressed;
	bool	 CtrlPressed;

	float	 Force; // @TODO rename
	
	EventInput(KeyCode key, KeyState state, bool shift = false, bool ctrl = false,  float Force = 0.f)
	: Key(key)
	, State(state)
	, Force(1.f)
	, ShiftPressed(shift)
	, CtrlPressed(ctrl)
	{
	}
};

typedef std::vector<KeyCode>			TVecInterruptKeys;
typedef TVecInterruptKeys::iterator		TVecInterruptKeysIterator;

#endif//__MessageMap_h__