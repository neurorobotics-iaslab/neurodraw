#include "neurodraw/Arc.h"

namespace neurodraw {

Arc::Arc(float radius, float thick, float angle, Color color) {
	
	this->create_arc(radius, thick, angle, 0.0f, 0.0f, color, NEURODRAW_CIRCLE_NPOINTS);

	this->set_color(color);
}

Arc::~Arc(void) {}

void Arc::create_arc(float radius, float thick, float angle, float cx, float cy, Color color,
					   unsigned int numpoints) {

	float r1, r2, cr, x, y;
	unsigned int numvert = 2*numpoints + 2;
	unsigned int numind =  2*numpoints + 2;
	GLenum primtype = GL_TRIANGLE_STRIP;

	this->set_primitive_type(primtype);
	
	r1 = radius;
	r2 = radius - thick;

	if (r2 <= 0 || thick < 0)
		return;

	// Create the circle, radius is expressed in width relative coordinates
	for (unsigned int i = 0; i<numvert; i++) {
		cr = i % 2 ? r2 : r1;
		this->add_index(i);
		x = cr * std::cos(static_cast<float>(i) * angle / static_cast<float>(numpoints)) +cx;
		y = cr * std::sin(static_cast<float>(i) * angle / static_cast<float>(numpoints)) +cy;
		this->add_point(x, y);
	}

}

}
