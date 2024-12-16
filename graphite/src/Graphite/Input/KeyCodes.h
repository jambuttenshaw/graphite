#pragma once

namespace Graphite
{

	using KeyCode = uint8_t;
	static inline constexpr KeyCode KEY_CODE_MAX = static_cast<uint8_t>(-1);

	namespace MouseButtons
	{
		static inline constexpr KeyCode KEY_LBUTTON = 0x01;
		static inline constexpr KeyCode KEY_RBUTTON = 0x02;
		static inline constexpr KeyCode KEY_MBUTTON = 0x04;			/* NOT contiguous with L & RBUTTON */
	}

	namespace KeyCodes
	{
		inline static constexpr KeyCode KEY_BACK = 0x08;
		inline static constexpr KeyCode KEY_TAB = 0x09;
		inline static constexpr KeyCode KEY_CLEAR = 0x0C;
		inline static constexpr KeyCode KEY_RETURN = 0x0D;
		inline static constexpr KeyCode KEY_SHIFT = 0x10;
		inline static constexpr KeyCode KEY_CONTROL = 0x11;
		inline static constexpr KeyCode KEY_MENU = 0x12;
		inline static constexpr KeyCode KEY_PAUSE = 0x13;
		inline static constexpr KeyCode KEY_CAPITAL = 0x14;
		inline static constexpr KeyCode KEY_ESCAPE = 0x1B;
		inline static constexpr KeyCode KEY_SPACE = 0x20;
		inline static constexpr KeyCode KEY_PRIOR = 0x21;
		inline static constexpr KeyCode KEY_NEXT = 0x22;
		inline static constexpr KeyCode KEY_END = 0x23;
		inline static constexpr KeyCode KEY_HOME = 0x24;
		inline static constexpr KeyCode KEY_LEFT = 0x25;
		inline static constexpr KeyCode KEY_UP = 0x26;
		inline static constexpr KeyCode KEY_RIGHT = 0x27;
		inline static constexpr KeyCode KEY_DOWN = 0x28;
		inline static constexpr KeyCode KEY_SELECT = 0x29;
		inline static constexpr KeyCode KEY_PRINT = 0x2A;
		inline static constexpr KeyCode KEY_SNAPSHOT = 0x2C;
		inline static constexpr KeyCode KEY_INSERT = 0x2D;
		inline static constexpr KeyCode KEY_DELETE = 0x2E;
		inline static constexpr KeyCode KEY_HELP = 0x2F;
		inline static constexpr KeyCode KEY_0 = 0x30;
		inline static constexpr KeyCode KEY_1 = 0x31;
		inline static constexpr KeyCode KEY_2 = 0x32;
		inline static constexpr KeyCode KEY_3 = 0x33;
		inline static constexpr KeyCode KEY_4 = 0x34;
		inline static constexpr KeyCode KEY_5 = 0x35;
		inline static constexpr KeyCode KEY_6 = 0x36;
		inline static constexpr KeyCode KEY_7 = 0x37;
		inline static constexpr KeyCode KEY_8 = 0x38;
		inline static constexpr KeyCode KEY_9 = 0x39;
		inline static constexpr KeyCode KEY_A = 0x41;
		inline static constexpr KeyCode KEY_B = 0x42;
		inline static constexpr KeyCode KEY_C = 0x43;
		inline static constexpr KeyCode KEY_D = 0x44;
		inline static constexpr KeyCode KEY_E = 0x45;
		inline static constexpr KeyCode KEY_F = 0x46;
		inline static constexpr KeyCode KEY_G = 0x47;
		inline static constexpr KeyCode KEY_H = 0x48;
		inline static constexpr KeyCode KEY_I = 0x49;
		inline static constexpr KeyCode KEY_J = 0x4A;
		inline static constexpr KeyCode KEY_K = 0x4B;
		inline static constexpr KeyCode KEY_L = 0x4C;
		inline static constexpr KeyCode KEY_M = 0x4D;
		inline static constexpr KeyCode KEY_N = 0x4E;
		inline static constexpr KeyCode KEY_O = 0x4F;
		inline static constexpr KeyCode KEY_P = 0x50;
		inline static constexpr KeyCode KEY_Q = 0x51;
		inline static constexpr KeyCode KEY_R = 0x52;
		inline static constexpr KeyCode KEY_S = 0x53;
		inline static constexpr KeyCode KEY_T = 0x54;
		inline static constexpr KeyCode KEY_U = 0x55;
		inline static constexpr KeyCode KEY_V = 0x56;
		inline static constexpr KeyCode KEY_W = 0x57;
		inline static constexpr KeyCode KEY_X = 0x58;
		inline static constexpr KeyCode KEY_Y = 0x59;
		inline static constexpr KeyCode KEY_Z = 0x5A;
		inline static constexpr KeyCode KEY_LWIN = 0x5B;
		inline static constexpr KeyCode KEY_RWIN = 0x5C;
		inline static constexpr KeyCode KEY_APPS = 0x5D;
		inline static constexpr KeyCode KEY_SLEEP = 0x5F;
		inline static constexpr KeyCode KEY_NUMPAD0 = 0x60;
		inline static constexpr KeyCode KEY_NUMPAD1 = 0x61;
		inline static constexpr KeyCode KEY_NUMPAD2 = 0x62;
		inline static constexpr KeyCode KEY_NUMPAD3 = 0x63;
		inline static constexpr KeyCode KEY_NUMPAD4 = 0x64;
		inline static constexpr KeyCode KEY_NUMPAD5 = 0x65;
		inline static constexpr KeyCode KEY_NUMPAD6 = 0x66;
		inline static constexpr KeyCode KEY_NUMPAD7 = 0x67;
		inline static constexpr KeyCode KEY_NUMPAD8 = 0x68;
		inline static constexpr KeyCode KEY_NUMPAD9 = 0x69;
		inline static constexpr KeyCode KEY_MULTIPLY = 0x6A;
		inline static constexpr KeyCode KEY_ADD = 0x6B;
		inline static constexpr KeyCode KEY_SEPARATOR = 0x6C;
		inline static constexpr KeyCode KEY_SUBTRACT = 0x6D;
		inline static constexpr KeyCode KEY_DECIMAL = 0x6E;
		inline static constexpr KeyCode KEY_DIVIDE = 0x6F;
		inline static constexpr KeyCode KEY_F1 = 0x70;
		inline static constexpr KeyCode KEY_F2 = 0x71;
		inline static constexpr KeyCode KEY_F3 = 0x72;
		inline static constexpr KeyCode KEY_F4 = 0x73;
		inline static constexpr KeyCode KEY_F5 = 0x74;
		inline static constexpr KeyCode KEY_F6 = 0x75;
		inline static constexpr KeyCode KEY_F7 = 0x76;
		inline static constexpr KeyCode KEY_F8 = 0x77;
		inline static constexpr KeyCode KEY_F9 = 0x78;
		inline static constexpr KeyCode KEY_F10 = 0x79;
		inline static constexpr KeyCode KEY_F11 = 0x7A;
		inline static constexpr KeyCode KEY_F12 = 0x7B;
		inline static constexpr KeyCode KEY_F13 = 0x7C;
		inline static constexpr KeyCode KEY_F14 = 0x7D;
		inline static constexpr KeyCode KEY_F15 = 0x7E;
		inline static constexpr KeyCode KEY_F16 = 0x7F;
		inline static constexpr KeyCode KEY_F17 = 0x80;
		inline static constexpr KeyCode KEY_F18 = 0x81;
		inline static constexpr KeyCode KEY_F19 = 0x82;
		inline static constexpr KeyCode KEY_F20 = 0x83;
		inline static constexpr KeyCode KEY_F21 = 0x84;
		inline static constexpr KeyCode KEY_F22 = 0x85;
		inline static constexpr KeyCode KEY_F23 = 0x86;
		inline static constexpr KeyCode KEY_F24 = 0x87;
		inline static constexpr KeyCode KEY_NUMLOCK = 0x90;
		inline static constexpr KeyCode KEY_SCROLL = 0x91;
		inline static constexpr KeyCode KEY_OEM_NEC_EQUAL = 0x92;   // '=' key on numpad
		inline static constexpr KeyCode KEY_LSHIFT = 0xA0;
		inline static constexpr KeyCode KEY_RSHIFT = 0xA1;
		inline static constexpr KeyCode KEY_LCONTROL = 0xA2;
		inline static constexpr KeyCode KEY_RCONTROL = 0xA3;
		inline static constexpr KeyCode KEY_LMENU = 0xA4;
		inline static constexpr KeyCode KEY_RMENU = 0xA5;
	}
}