#include "neurodraw/Rectangle.h"

namespace neurodraw {


Rectangle::Rectangle(float width, float hight, bool is_filled, Color color) {

	this->add_point(-width/2.0f, -hight/2.0f);
	this->add_point(-width/2.0f,  hight/2.0f);
	this->add_point( width/2.0f,  hight/2.0f);
	this->add_point( width/2.0f, -hight/2.0f);

	for(auto i = 0; i<4; i++)
		this->add_index(i);

	if(is_filled)
		this->set_primitive_type(GL_TRIANGLE_FAN);

	this->set_color(color);

}

Rectangle::~Rectangle(void) {}


}
