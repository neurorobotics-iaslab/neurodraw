#ifndef NEURODRAW_DISPLAY_H_
#define NEURODRAW_DISPLAY_H_

#include <string>
#include <unordered_set>
#include <chrono>
#include <functional>
#include <drawtk.h>
#include <dtk_event.h>
#include "neurodraw/Engine.h"
#include "neurodraw/Shape.h"
#include "neurodraw/Window.h"
#include "neurodraw/EventHandler.h"
#include "neurodraw/neurochrono.h"

namespace neurodraw {

class Display : public Engine {

	public:
		Display(const std::string& title = "untitled", float rate = 60);
		virtual ~Display(void);

		void run(void);

		bool draw(Shape* shape);
		bool clear(Shape* shape);
		void size(unsigned int* width, unsigned int* height);

		void quit(void);

	private:
		void update(void);
		void check_refresh_rate(int elapsed_msecs);

	private:
		Window* window_;

	private:
		std::unordered_set<Shape*> list_;
		float rate_;
		const float rate_tollerance_  = 5;
		

	friend class EventHandler;
};

}



#endif
