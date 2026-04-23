#include "neurodraw/RegularShape.h"

namespace neurodraw {

RegularShape::RegularShape(float radius, unsigned int nvertices, bool is_filled, Color color) {

	this->create_regular_shape(radius, 0.0f, 0.0f, color, is_filled, nvertices);
	this->set_color(color);

}

RegularShape::~RegularShape(void) {}

void RegularShape::create_regular_shape(float radius, float cx, float cy, Color color,
								  bool is_filled, unsigned int numpoints) {

	unsigned int i, j;
	float x, y;
	unsigned int numvert = is_filled ? numpoints+1 : numpoints;
	
	if(is_filled) {
		this->set_fill_primitive_type(GL_TRIANGLE_FAN);
		this->set_stroke_primitive_type(GL_LINE_LOOP);
	} else {
		this->set_primitive_type(GL_LINE_LOOP);
	}

	i = j = 0;
	if(is_filled) {
		this->add_point(cx, cy);
		this->add_fill_index(0);
		i++;
	}

	while(i < numvert) {
		x = radius * std::cos(static_cast<float>(j)*2.0f *M_PI / static_cast<float>(numpoints)) + cx;
		y = radius * std::sin(static_cast<float>(j)*2.0f *M_PI / static_cast<float>(numpoints)) + cy;
		this->add_point(x, y);
		if(is_filled) {
			this->add_fill_index(i);
			this->add_stroke_index(i);
		} else {
			this->add_index(i);
		}
		i++;
		j++;
	}

	if(is_filled)
		this->add_fill_index(1);
}




}
