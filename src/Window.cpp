#include "neurodraw/Window.h"

namespace neurodraw {

Window::Window(const std::string& title) : title_(title) {
	this->lock();
	this->is_open_ = false;
	this->unlock();
}

Window::~Window(void) {
	printf("[window] Destructor\n");
	this->lock();
	bool is_open = this->is_open_;
	this->unlock();

	if(is_open == true)
		this->close();

}

void Window::open(void) {

	this->lock();
	this->wnd_ = dtk_create_window(this->default_width_, this->default_height_,
								   this->default_x_, this->default_y_, 
								   this->default_bpp_, this->title_.c_str());
	this->is_open_ = true;
	this->unlock();
}

void Window::make_current(void) {
	this->lock();
	if(this->wnd_ != nullptr)
		dtk_make_current_window(this->wnd_);
	this->unlock();
}

void Window::close(void) {
	this->lock();
	if(this->wnd_ != nullptr)
		dtk_close(this->wnd_);
	this->is_open_ = false;
	this->unlock();
}

void Window::clear(void) {
	this->lock();
	if(this->wnd_ != nullptr)
		dtk_clear_screen(this->wnd_);
	this->unlock();
}

void Window::update(void) {
	this->lock();
	if(this->wnd_ != nullptr)
		dtk_update_screen(this->wnd_);
	this->unlock();
}

bool Window::is_open(void) {
	bool ret;
	this->lock();
	ret = this->is_open_;
	this->unlock();
	return ret;
}

void Window::process_events(void) {
	this->lock();
	dtk_process_events(this->wnd_);
	this->unlock();
}

void Window::set_event_handler(DTKEvtProc func) {
	this->lock();
	dtk_set_event_handler(this->wnd_, func);
	this->unlock();
}

void Window::lock(void) {
	this->mutex_.lock();
}

void Window::unlock(void) {
	this->mutex_.unlock();
}

void Window::getsize(unsigned int* width, unsigned int* height) {
	//this->lock();
	dtk_window_getsize(this->wnd_, width, height);
	//this->unlock();
}


}
