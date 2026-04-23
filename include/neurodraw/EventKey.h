#ifndef NEURODRAW_EVENTKEY_H_
#define NEURODRAW_EVENTKEY_H_

#include <SDL2/SDL.h>

namespace neurodraw {

class EventKey {
	public:
		static constexpr int UNKNOWN = SDLK_UNKNOWN;
		static constexpr int FIRST = SDLK_UNKNOWN;
		static constexpr int BACKSPACE = SDLK_BACKSPACE;
		static constexpr int TAB = SDLK_TAB;
		static constexpr int CLEAR = SDLK_CLEAR;
		static constexpr int RETURN = SDLK_RETURN;
		static constexpr int PAUSE = SDLK_PAUSE;
		static constexpr int ESCAPE = SDLK_ESCAPE;
		static constexpr int SPACE = SDLK_SPACE;
		static constexpr int EXCLAIM = SDLK_EXCLAIM;
		static constexpr int QUOTEDBL = SDLK_QUOTEDBL;
		static constexpr int HASH = SDLK_HASH;
		static constexpr int DOLLAR = SDLK_DOLLAR;
		static constexpr int AMPERSAND = SDLK_AMPERSAND;
		static constexpr int QUOTE = SDLK_QUOTE;
		static constexpr int LEFTPAREN = SDLK_LEFTPAREN;
		static constexpr int RIGHTPAREN = SDLK_RIGHTPAREN;
		static constexpr int ASTERISK = SDLK_ASTERISK;
		static constexpr int PLUS = SDLK_PLUS;
		static constexpr int COMMA = SDLK_COMMA;
		static constexpr int MINUS = SDLK_MINUS;
		static constexpr int PERIOD = SDLK_PERIOD;
		static constexpr int SLASH = SDLK_SLASH;
		static constexpr int NUM_0 = SDLK_0;
		static constexpr int NUM_1 = SDLK_1;
		static constexpr int NUM_2 = SDLK_2;
		static constexpr int NUM_3 = SDLK_3;
		static constexpr int NUM_4 = SDLK_4;
		static constexpr int NUM_5 = SDLK_5;
		static constexpr int NUM_6 = SDLK_6;
		static constexpr int NUM_7 = SDLK_7;
		static constexpr int NUM_8 = SDLK_8;
		static constexpr int NUM_9 = SDLK_9;
		static constexpr int COLON = SDLK_COLON;
		static constexpr int SEMICOLON = SDLK_SEMICOLON;
		static constexpr int LESS = SDLK_LESS;
		static constexpr int EQUALS = SDLK_EQUALS;
		static constexpr int GREATER = SDLK_GREATER;
		static constexpr int QUESTION = SDLK_QUESTION;
		static constexpr int AT = SDLK_AT;
		static constexpr int LEFTBRACKET = SDLK_LEFTBRACKET;
		static constexpr int BACKSLASH = SDLK_BACKSLASH;
		static constexpr int RIGHTBRACKET = SDLK_RIGHTBRACKET;
		static constexpr int CARET = SDLK_CARET;
		static constexpr int UNDERSCORE = SDLK_UNDERSCORE;
		static constexpr int BACKQUOTE = SDLK_BACKQUOTE;
		static constexpr int a = SDLK_a;
		static constexpr int b = SDLK_b;
		static constexpr int c = SDLK_c;
		static constexpr int d = SDLK_d;
		static constexpr int e = SDLK_e;
		static constexpr int f = SDLK_f;
		static constexpr int g = SDLK_g;
		static constexpr int h = SDLK_h;
		static constexpr int i = SDLK_i;
		static constexpr int j = SDLK_j;
		static constexpr int k = SDLK_k;
		static constexpr int l = SDLK_l;
		static constexpr int m = SDLK_m;
		static constexpr int n = SDLK_n;
		static constexpr int o = SDLK_o;
		static constexpr int p = SDLK_p;
		static constexpr int q = SDLK_q;
		static constexpr int r = SDLK_r;
		static constexpr int s = SDLK_s;
		static constexpr int t = SDLK_t;
		static constexpr int u = SDLK_u;
		static constexpr int v = SDLK_v;
		static constexpr int w = SDLK_w;
		static constexpr int x = SDLK_x;
		static constexpr int y = SDLK_y;
		static constexpr int z = SDLK_z;
		static constexpr int DELETE = SDLK_DELETE;
		static constexpr int KP0 = SDLK_KP_0;
		static constexpr int KP1 = SDLK_KP_1;
		static constexpr int KP2 = SDLK_KP_2;
		static constexpr int KP3 = SDLK_KP_3;
		static constexpr int KP4 = SDLK_KP_4;
		static constexpr int KP5 = SDLK_KP_5;
		static constexpr int KP6 = SDLK_KP_6;
		static constexpr int KP7 = SDLK_KP_7;
		static constexpr int KP8 = SDLK_KP_8;
		static constexpr int KP9 = SDLK_KP_9;
		static constexpr int KP_PERIOD = SDLK_KP_PERIOD;
		static constexpr int KP_DIVIDE = SDLK_KP_DIVIDE;
		static constexpr int KP_MULTIPLY = SDLK_KP_MULTIPLY;
		static constexpr int KP_MINUS = SDLK_KP_MINUS;
		static constexpr int KP_PLUS = SDLK_KP_PLUS;
		static constexpr int KP_ENTER = SDLK_KP_ENTER;
		static constexpr int KP_EQUALS = SDLK_KP_EQUALS;
		static constexpr int UP = SDLK_UP;
		static constexpr int DOWN = SDLK_DOWN;
		static constexpr int RIGHT = SDLK_RIGHT;
		static constexpr int LEFT = SDLK_LEFT;
		static constexpr int INSERT = SDLK_INSERT;
		static constexpr int HOME = SDLK_HOME;
		static constexpr int END = SDLK_END;
		static constexpr int PAGEUP = SDLK_PAGEUP;
		static constexpr int PAGEDOWN = SDLK_PAGEDOWN;
		static constexpr int F1 = SDLK_F1;
		static constexpr int F2 = SDLK_F2;
		static constexpr int F3 = SDLK_F3;
		static constexpr int F4 = SDLK_F4;
		static constexpr int F5 = SDLK_F5;
		static constexpr int F6 = SDLK_F6;
		static constexpr int F7 = SDLK_F7;
		static constexpr int F8 = SDLK_F8;
		static constexpr int F9 = SDLK_F9;
		static constexpr int F10 = SDLK_F10;
		static constexpr int F11 = SDLK_F11;
		static constexpr int F12 = SDLK_F12;
		static constexpr int F13 = SDLK_F13;
		static constexpr int F14 = SDLK_F14;
		static constexpr int F15 = SDLK_F15;
		static constexpr int NUMLOCK = SDLK_NUMLOCKCLEAR;
		static constexpr int CAPSLOCK = SDLK_CAPSLOCK;
		static constexpr int SCROLLOCK = SDLK_SCROLLLOCK;
		static constexpr int RSHIFT = SDLK_RSHIFT;
		static constexpr int LSHIFT = SDLK_LSHIFT;
		static constexpr int RCTRL = SDLK_RCTRL;
		static constexpr int LCTRL = SDLK_LCTRL;
		static constexpr int RALT = SDLK_RALT;
		static constexpr int LALT = SDLK_LALT;
		static constexpr int RMETA = SDLK_RGUI;
		static constexpr int LMETA = SDLK_LGUI;
		static constexpr int LSUPER = SDLK_LGUI;
		static constexpr int RSUPER = SDLK_RGUI;
		static constexpr int MODE = SDLK_MODE;
		static constexpr int HELP = SDLK_HELP;
		static constexpr int PRINT = SDLK_PRINTSCREEN;
		static constexpr int SYSREQ = SDLK_SYSREQ;
		static constexpr int BREAK = SDLK_PAUSE;
		static constexpr int MENU = SDLK_APPLICATION;
		static constexpr int POWER = SDLK_POWER;
#ifdef SDLK_EURO
		static constexpr int EURO = SDLK_EURO;
#else
		static constexpr int EURO = SDLK_UNKNOWN;
#endif
		static constexpr int UNDO = SDLK_UNDO;
};

}

#endif
