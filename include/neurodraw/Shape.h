#ifndef NEURODRAW_SHAPE_H_
#define NEURODRAW_SHAPE_H_

#include <array>
#include <cmath>
#include <mutex>
#include <vector>

#include <SDL2/SDL_opengl.h>

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
		void rotate(float deg, float refx, float refy, bool hold = false);
		void relrotate(float ddeg, float refx, float refy, bool hold = false);
		virtual void hide(void);
		virtual void show(void);
		float x(void);
		float y(void);
		float rotation(void);
		virtual float alpha(void);

		// Color functions
		void set_color(Color color);
		void set_color(Color color, unsigned int index);
		virtual void set_alpha(float alpha);
		void set_alpha(float alpha, unsigned int index);
		void set_fill_color(Color color);
		void set_fill_color(Color color, unsigned int index);
		void set_fill_alpha(float alpha);
		void set_fill_alpha(float alpha, unsigned int index);
		void set_stroke_color(Color color);
		void set_stroke_color(Color color, unsigned int index);
		void set_stroke_alpha(float alpha);
		void set_stroke_alpha(float alpha, unsigned int index);
		void set_stroke_width(float width);
		float stroke_width(void);

		// Geometry functions
		unsigned int num_points(void);
		std::vector<float> points(void);
		std::vector<unsigned int> indices(void);
		std::vector<float> colors(void);
		void set_primitive_type(GLenum type);
		void set_fill_primitive_type(GLenum type);
		void set_stroke_primitive_type(GLenum type);
		void add_outline_index(unsigned int index);

		virtual void draw(void);
		virtual void local_bounds(float* min_x, float* min_y,
		                         float* max_x, float* max_y);
		void bounds(float* min_x, float* min_y, float* max_x, float* max_y);
		virtual bool contains(float x, float y);
		virtual Shape* hit(float x, float y);
		void lock(void);
		void unlock(void);

	protected:
		Shape(void);
		void add_point(float x, float y);
		void add_index(unsigned int index);
		void add_fill_index(unsigned int index);
		void add_stroke_index(unsigned int index);
		void enable_stroke(GLenum type);
		bool uses_fill_mesh(void) const;
		bool has_stroke_mesh(void) const;
		void current_transform(float* x, float* y, float* rot);

	private:
		void set_mesh_color(std::vector<float>& colors, Color color);
		void set_mesh_color(std::vector<float>& colors, Color color, unsigned int index);
		void set_mesh_alpha(std::vector<float>& colors, float alpha);
		void set_mesh_alpha(std::vector<float>& colors, float alpha, unsigned int index);

	private:
		std::vector<float> points_;
		std::vector<unsigned int> indices_;
		std::vector<unsigned int> outline_indices_;

	private:
		float x_;
		float y_;
		float rot_;
		GLenum primtype_;
		GLenum outline_primtype_;
		bool primary_is_fill_;
		bool has_outline_;
		float stroke_width_;

		std::vector<float> colors_;
		std::vector<float> outline_colors_;

		std::mutex mutex_;
};

}


#endif
