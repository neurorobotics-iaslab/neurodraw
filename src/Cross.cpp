#include "neurodraw/Cross.h"


namespace neurodraw {

Cross::Cross(float size, float thick, Color color) {


	this->set_primitive_type(GL_QUADS);

	this->add_point( -size/2.0f, -thick/2.0f);
	this->add_point(  -size/2.0f, thick/2.0f);
	this->add_point(   size/2.0f, thick/2.0f);
	this->add_point(   size/2.0f, -thick/2.0f);


	this->add_point( -thick/2.0f, -size/2.0f);
	this->add_point( -thick/2.0f, size/2.0f);
	this->add_point(  thick/2.0f, size/2.0f);
	this->add_point(  thick/2.0f, -size/2.0f);
	
	for(auto i = 0; i < this->points().size() / 2.0f; i++) 
		this->add_index(i);
	

	this->set_color(color);
}

Cross::~Cross(void) {}



}
