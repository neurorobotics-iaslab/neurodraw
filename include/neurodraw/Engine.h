#ifndef NEURODRAW_ENGINE_H_
#define NEURODRAW_ENGINE_H_

#include <string>
#include <list>
#include <chrono>
#include <functional>

#include "neurodraw/common.h"
#include "neurodraw/Thread.h"
#include "neurodraw/Shape.h"
#include "neurodraw/Window.h"
#include "neurochrono/Timer.h"
#include "neurochrono/Rate.h"

namespace neurodraw {

class Engine : public Thread {

	public:
		Engine(const std::string& wtitle = "untitled", float fps = NEURODRAW_FPS);
		~Engine(void);

		bool add(Shape* shape);
		bool erase(unsigned int pos);
		void clear(void);

		void winsize(unsigned int* width, unsigned int* height);
		float fps(void);
		float real_fps(void);
		void  set_fps_tolerance(float tolerance);
		float fps_minimum(void);

		void quit(void);

		void on_keyboard(std::function<void(const KeyboardEvent& kevt)> func);
		void on_mouse(std::function<void(const MouseEvent& mevt)> func);
		
		template<class T>
		void on_keyboard( void(T::*fp)(const KeyboardEvent& kevt), T* obj);

		template<class T>
		void on_mouse( void(T::*fp)(const MouseEvent& mevt), T* obj);


	private:
		void update(void);
		void run(void);
		void callback_quit(void);
		void callback_redraw(void);


	private:
		Window* window_;

	private:
		std::list<Shape*> list_;
		float fps_;
		float fps_minimum_;
		

};

template<class T>
void Engine::on_keyboard( void(T::*fp)(const KeyboardEvent& kevt), T* obj) {
	this->lock();
	this->window_->on_keyboard(fp, obj);
	this->unlock();
}

template<class T>
void Engine::on_mouse( void(T::*fp)(const MouseEvent& mevt), T* obj) {
	this->lock();
	this->window_->on_mouse(std::bind(fp, obj, std::placeholders::_1));
	this->unlock();
}


}



#endif
