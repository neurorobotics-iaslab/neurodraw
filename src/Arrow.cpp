#include "neurodraw/Arrow.h"

namespace neurodraw {

Arrow::Arrow(float width, float height, float headwidth, float bodyheight, bool is_filled, Color color) {


	if(is_filled)
		this->create_arrow_filled(width, height, headwidth, bodyheight);
	else
		this->create_arrow_stroke(width, height, headwidth, bodyheight);


	this->set_color(color);
	

}

Arrow::Arrow(float width, float height, bool is_filled, Color color) {

	float headwidth  = width/4.0f;
	float bodyheight = height/2.0f; 

	if(is_filled)
		this->create_arrow_filled(width, height, headwidth, bodyheight);
	else
		this->create_arrow_stroke(width, height, headwidth, bodyheight);


	this->set_color(color);
}
	

Arrow::~Arrow(void) {}


void Arrow::create_arrow_filled(float width, float height, float headsize, float bodysize) {
	
	this->set_primitive_type(GL_TRIANGLES);

	headsize = headsize > width ? width : headsize;

	this->add_point( -width/2.0f, 0.0f);
	this->add_index(0);
	this->add_point( -width/2.0f + headsize, height/2.0f);
	this->add_index(1);
	this->add_point( -width/2.0f + headsize, -height/2.0f);
	this->add_index(2);
	
	this->add_point( -width/2.0f + headsize, bodysize/2.0f);
	this->add_index(3);
	this->add_point(  width/2.0f, bodysize/2.0f);
	this->add_index(4);
	this->add_point(  width/2.0f, -bodysize/2.0f);
	this->add_index(5);
	this->add_point( -width/2.0f + headsize, -bodysize/2.0f);
	this->add_index(5);
	this->add_index(6);
	this->add_index(3);

}

void Arrow::create_arrow_stroke(float width, float height, float headsize, float bodysize) {
	
	this->set_primitive_type(GL_LINE_LOOP);

	headsize = headsize > width ? width : headsize;

	this->add_point( -width/2.0f, 0.0f);
	this->add_point( -width/2.0f + headsize, height/2.0f);
	this->add_point( -width/2.0f + headsize, bodysize/2.0f);
	this->add_point(  width/2.0f, bodysize/2.0f);
	this->add_point(  width/2.0f, -bodysize/2.0f);
	this->add_point( -width/2.0f + headsize, -bodysize/2.0f);
	this->add_point( -width/2.0f + headsize, -height/2.0f);

	for(auto i = 0; i < this->num_points(); i++)
		this->add_index(i);

}


}
