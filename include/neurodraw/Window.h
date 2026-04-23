#ifndef NEURODRAW_WINDOW_H_
#define NEURODRAW_WINDOW_H_

#include <array>
#include <functional>
#include <mutex>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "neurodraw/Palette.h"

namespace neurodraw {

struct MouseEvent {
	MouseEvent(void) : button(0), state(0), x(0), y(0) {}

	MouseEvent(const SDL_Event& evt) : button(0), state(0), x(0), y(0) {
		switch(evt.type) {
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				button = evt.button.button;
				state = evt.button.state;
				x = evt.button.x;
				y = evt.button.y;
				break;

			case SDL_MOUSEMOTION:
				state = 0;
				x = evt.motion.x;
				y = evt.motion.y;
				break;
		}
	}

	unsigned int button;
	unsigned int state;
	int x;
	int y;
};

struct KeyboardEvent {
	KeyboardEvent(void) : state(0), sym(0), mod(0) {}

	KeyboardEvent(const SDL_Event& evt) {
		state = (evt.key.state == SDL_PRESSED) ? 1 : 0;
		sym = evt.key.keysym.sym;
		mod = evt.key.keysym.mod;
	}

	unsigned int state;
	int sym;
	unsigned int mod;
};

class Window {

	public:
		Window(const std::string& caption = "untitled");
		~Window(void);

		bool open(void);
		void close(void);
		void clear(void);
		void update(void);
		bool is_open(void);

		void set_color(Color color);
		void size(unsigned int* width, unsigned int* height);

		void process_events(void);

		void on_quit(std::function<void(void)> func);
		void on_redraw(std::function<void(void)> func);
		void on_keyboard(std::function<void(const KeyboardEvent& kevt)> func);
		void on_mouse(std::function<void(const MouseEvent& mevt)> func);

		template<class T>
		void on_quit( void(T::*fp)(void), T* obj);

		template<class T>
		void on_redraw( void(T::*fp)(void), T* obj);

		template<class T>
		void on_keyboard( void(T::*fp)(const KeyboardEvent& kevt), T* obj);

		template<class T>
		void on_mouse( void(T::*fp)(const MouseEvent& mevt), T* obj);

	protected:
		void lock(void);
		void unlock(void);

	private:
		int init_opengl_state(void);
		int resize_window(int width, int height, int fullscreen);
		int open_window(int width, int height, const std::string& caption);

	private:
		unsigned int width_;
		unsigned int height_;
		unsigned int x_;
		unsigned int y_;
		unsigned int bpp_;
		std::string caption_;
		std::array<float, 4> color_;

		SDL_Window* window_;
		SDL_GLContext context_;

		std::function<void(void)> on_quit_func_;
		std::function<void(void)> on_redraw_func_;
		std::function<void(const KeyboardEvent& kevt)> on_keyboard_func_;
		std::function<void(const MouseEvent& mevt)> on_mouse_func_;

		bool is_open_;
		std::mutex mutex_;
};

template<class T>
void Window::on_quit( void(T::*fp)(void), T* obj) {
	this->on_quit(std::bind(fp, obj));
}

template<class T>
void Window::on_redraw( void(T::*fp)(void), T* obj) {
	this->on_redraw(std::bind(fp, obj));
}

template<class T>
void Window::on_keyboard( void(T::*fp)(const KeyboardEvent& kevt), T* obj) {
	this->on_keyboard(std::bind(fp, obj, std::placeholders::_1));
}

template<class T>
void Window::on_mouse( void(T::*fp)(const MouseEvent& mevt), T* obj) {
	this->on_mouse(std::bind(fp, obj, std::placeholders::_1));
}

}

#endif
