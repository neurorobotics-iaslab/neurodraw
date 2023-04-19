#include "neurodraw/EventHandler.h"

namespace neurodraw {

EventHandler* PtrEventHandler_ = nullptr;

int func_event_handler(dtk_hwnd wnd, int type, const union dtk_event* evt) {

	if(PtrEventHandler_ != nullptr)
		PtrEventHandler_->process_events(type, evt);
		//PtrEventHandler_->process_events(wnd, type, evt);

	return 0;
}

EventHandler::EventHandler(void) {
	PtrEventHandler_     = this;
	this->on_quit   	 = nullptr;
	this->on_redraw 	 = nullptr;
	this->on_keyboard    = nullptr;
	this->on_mousebutton = nullptr;
	this->on_mousemotion = nullptr;
}

EventHandler::~EventHandler(void) {
	PtrEventHandler_ = nullptr;
}

//void EventHandler::process_events(dtk_hwnd wnd, int type, const union dtk_event* evt) {
void EventHandler::process_events(int type, const union dtk_event* evt) {

	switch(type) {
		case DTK_EVT_REDRAW:
			if(this->on_redraw != nullptr)
				this->on_redraw();
			break;
		case DTK_EVT_QUIT:
			if(this->on_quit != nullptr)
				this->on_quit();
			break;
		case DTK_EVT_KEYBOARD:
			if(this->on_keyboard != nullptr)
				this->on_keyboard(EventKeyboard(evt->key));
			break;
		case DTK_EVT_MOUSEBUTTON:
			if(this->on_mousebutton != nullptr)
				this->on_mousebutton(EventMouse(evt->mouse));

			break;
		case DTK_EVT_MOUSEMOTION:
			if(this->on_mousemotion != nullptr)
				this->on_mousemotion(EventMouse(evt->mouse));
			break;
		default:
			break;
	}

}






}
