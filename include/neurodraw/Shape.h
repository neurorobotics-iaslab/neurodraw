#ifndef NEURODRAW_SHAPE_H_
#define NEURODRAW_SHAPE_H_

#include <string.h>
#include <mutex>
#include <drawtk.h>
#include <dtk_colors.h>

#include "neurodraw/Palette.h"

namespace neurodraw {

// Forward declaration for class Display
class Display;

class Shape {
	
	public:

		friend class Display;

		virtual ~Shape(void);
		Shape(const Shape&) = delete;
		Shape& operator=(const Shape&) = delete;

		void move(float x, float y);
		void rmove(float dx, float dy);
		void rotate(float deg);
		void rrotate(float ddeg);
		void hide(void);
		void show(void);
		void fill(void);
		void unfill(void);
		
		
		void set_color(Color color, float alpha = 1.0f);
		void set_alpha(float alpha);

		Color get_color(void);
		float get_alpha(void);

	protected:
		Shape(void);

	private:
		std::array<float, 4> dtk_color_array(Color color, float alpha);
		void render(void);
		void lock(void);
		void unlock(void);

	protected:
		dtk_hshape shape_;
		Color color_;
		bool is_filled_;
		float x_;
		float y_;
		float z_;

	private:
		std::mutex mutex_;

};


}


#endif
