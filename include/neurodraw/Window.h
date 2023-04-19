#ifndef NEURODRAW_WINDOW_
#define NEURODRAW_WINDOW_

#include <mutex>
#include <functional>
#include <drawtk.h>
#include <dtk_event.h>

namespace neurodraw {

class Window {

	public:
		Window(const std::string& title = "untitled");
		~Window(void);

		void open(void);
		void make_current(void);
		void close(void);
		void clear(void);
		void getsize(unsigned int* width, unsigned int* height);
		void update(void);
		bool is_open(void);
		void process_events(void);
		void set_event_handler(DTKEvtProc func);

		void lock(void);
		void unlock(void);

	private:
		dtk_hwnd wnd_;
		std::mutex mutex_;
		bool is_open_;
		
		const std::string title_;
		const int default_width_  = 1024;
		const int default_height_ = 768;
		const int default_x_      = 0;
		const int default_y_      = 0;
		const int default_bpp_ 	  = 24;

};


}


#endif
