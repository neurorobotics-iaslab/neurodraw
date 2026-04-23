#include "neurodraw/Rectangle.h"

namespace neurodraw {


Rectangle::Rectangle(float width, float hight, bool is_filled, Color color) {

	this->add_point(-width/2.0f, -hight/2.0f);
	this->add_point(-width/2.0f,  hight/2.0f);
	this->add_point( width/2.0f,  hight/2.0f);
	this->add_point( width/2.0f, -hight/2.0f);

	if(is_filled) {
		this->set_fill_primitive_type(GL_TRIANGLE_FAN);
		this->set_stroke_primitive_type(GL_LINE_LOOP);
		for(unsigned int i = 0; i < 4; ++i) {
			this->add_fill_index(i);
			this->add_stroke_index(i);
		}
	} else {
		this->set_primitive_type(GL_LINE_LOOP);
		for(unsigned int i = 0; i < 4; ++i)
			this->add_index(i);
	}

	this->set_color(color);

}

Rectangle::~Rectangle(void) {}


}
