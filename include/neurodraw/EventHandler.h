#ifndef NEURODRAW_EVENT_HANDLER_H_
#define NEURODRAW_EVENT_HANDLER_H_

#include <functional>
#include <dtk_event.h>

namespace neurodraw {

typedef dtk_keyevent EventKeyboard;
typedef dtk_mouseevent EventMouse;
class Window;

class EventHandler {

	public:
		EventHandler(void);
		~EventHandler(void);


	public:
		std::function<void(void)> on_quit;
		std::function<void(void)> on_redraw;
		std::function<void(EventKeyboard)> on_keyboard;
		std::function<void(EventMouse)> on_mousebutton;
		std::function<void(EventMouse)> on_mousemotion;


	private:
		//void process_events(dtk_hwnd wnd, int type, const union dtk_event* evt);
		void process_events(int type, const union dtk_event* evt);



	friend int func_event_handler(dtk_hwnd wnd, int type, const union dtk_event* evt);


};

// Global function to manage the C-style signature of the dtk_event callback
int func_event_handler(dtk_hwnd wnd, int type, const union dtk_event* evt);




}

#endif
