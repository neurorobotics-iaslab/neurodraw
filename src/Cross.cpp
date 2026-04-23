#include "neurodraw/Cross.h"


namespace neurodraw {

Cross::Cross(float size, float thick, Color color) {


	this->set_fill_primitive_type(GL_TRIANGLES);
	this->set_stroke_primitive_type(GL_LINES);

	this->add_point( -size/2.0f, -thick/2.0f);
	this->add_point(  -size/2.0f, thick/2.0f);
	this->add_point(   size/2.0f, thick/2.0f);
	this->add_point(   size/2.0f, -thick/2.0f);


	this->add_point( -thick/2.0f, -size/2.0f);
	this->add_point( -thick/2.0f, size/2.0f);
	this->add_point(  thick/2.0f, size/2.0f);
	this->add_point(  thick/2.0f, -size/2.0f);

	this->add_fill_index(0);
	this->add_fill_index(1);
	this->add_fill_index(2);
	this->add_fill_index(0);
	this->add_fill_index(2);
	this->add_fill_index(3);

	this->add_fill_index(4);
	this->add_fill_index(5);
	this->add_fill_index(6);
	this->add_fill_index(4);
	this->add_fill_index(6);
	this->add_fill_index(7);

	this->add_stroke_index(0);
	this->add_stroke_index(1);
	this->add_stroke_index(1);
	this->add_stroke_index(2);
	this->add_stroke_index(2);
	this->add_stroke_index(3);
	this->add_stroke_index(3);
	this->add_stroke_index(0);

	this->add_stroke_index(4);
	this->add_stroke_index(5);
	this->add_stroke_index(5);
	this->add_stroke_index(6);
	this->add_stroke_index(6);
	this->add_stroke_index(7);
	this->add_stroke_index(7);
	this->add_stroke_index(4);

	this->set_stroke_width(2.0f);

	this->set_color(color);
}

Cross::~Cross(void) {}



}
