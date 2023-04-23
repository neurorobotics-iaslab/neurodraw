#include "neurodraw/Line.h"

namespace neurodraw {


Line::Line(float x1, float y1, float x2, float y2, Color color) {

	this->set_primitive_type(GL_LINES);

	this->add_point(x1, y1);
	this->add_point(x2, y2);

	this->add_index(0);
	this->add_index(1);

	this->set_color(color);
}

Line::Line(float length, Color color) {

	this->set_primitive_type(GL_LINES);

	this->add_point(-length/2.0f, 0.0f);
	this->add_point( length/2.0f, 0.0f);

	this->add_index(0);
	this->add_index(1);

	this->set_color(color);
}

Line::~Line(void) {}


}
