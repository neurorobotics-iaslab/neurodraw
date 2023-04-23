#ifndef NEURODRAW_SHAPE_H_
#define NEURODRAW_SHAPE_H_

#include <vector>
#include <array>
#include <mutex>
#include <SDL/SDL_opengl.h>

#include "neurodraw/Palette.h"

namespace neurodraw {

class Shape {

	public:
		virtual ~Shape(void);
		Shape(const Shape&) = delete;
		Shape& operator=(const Shape&) = delete;

		// Manipulation functions
		void move(float x, float y);
		void relmove(float dx, float dy);
		void rotate(float deg);
		void relrotate(float ddeg);
		void hide(void);
		void show(void);

		// Color functions
		void set_color(Color color);
		void set_color(Color color, unsigned int index);
		void set_alpha(float alpha);
		void set_alpha(float alpha, unsigned int index);

		// Geometry functions
		unsigned int num_points(void);
		std::vector<float> points(void);
		std::vector<unsigned int> indices(void);
		std::vector<float> colors(void);
		void set_primitive_type(GLenum type);
		
		void draw(void);
		void lock(void);
		void unlock(void);

	protected:
		Shape(void);
		void add_point(float x, float y);
		void add_index(unsigned int index);

	private:
		std::vector<float> 		  points_;
		std::vector<unsigned int> indices_;

	private:
		float x_;
		float y_;
		float rot_;
		GLenum primtype_;
		
		std::vector<float> colors_;


		std::mutex mutex_;
};


}


#endif
