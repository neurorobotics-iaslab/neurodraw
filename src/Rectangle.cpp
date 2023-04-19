#include "neurodraw/Rectangle.h"

namespace neurodraw {


Rectangle::Rectangle(float width, float height, Color color, bool is_filled)  {
	this->color_ 	 = color;
	this->is_filled_ = is_filled;


	this->shape_ = dtk_create_rectangle_hw(this->shape_, this->x_, this->y_, 
			   								 width, height, this->is_filled_,
											 dtk_white);

	//this->set_color(color, 1.0f);

}

Rectangle::~Rectangle(void) {}



}
