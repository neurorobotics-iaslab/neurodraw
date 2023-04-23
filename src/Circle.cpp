#include "neurodraw/Circle.h"

namespace neurodraw {

Circle::Circle(float radius, bool is_filled, Color color) : RegularShape(radius, NEURODRAW_CIRCLE_NPOINTS, is_filled, color) { }

Circle::~Circle(void) {}


}
