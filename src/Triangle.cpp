#include "neurodraw/Triangle.h"

namespace neurodraw {

Triangle::Triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool is_full, Color color) {


	if(is_full) 
		this->set_primitive_type(GL_TRIANGLES);
	else
		this->set_primitive_type(GL_LINE_LOOP);

	this->add_point(x1, y1);
	this->add_point(x2, y2);
	this->add_point(x3, y3);

	for(auto i = 0; i < this->num_points(); i++)
		this->add_index(i);

	this->set_color(color);
}

Triangle::Triangle(float width, float height, bool is_full, Color color) {

	if(is_full) 
		this->set_primitive_type(GL_TRIANGLES);
	else
		this->set_primitive_type(GL_LINE_LOOP);

	this->add_point(-width/2.0f, -height/2.0f);
	this->add_point( 0.0, height/2.0f);
	this->add_point( width/2.0f, -height/2.0f);
	
	for(auto i = 0; i < this->num_points(); i++)
		this->add_index(i);

	this->set_color(color);
}

Triangle::~Triangle(void) {}



}
