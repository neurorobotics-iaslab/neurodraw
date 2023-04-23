#include "neurodraw/Ring.h"

namespace neurodraw {

Ring::Ring(float radius, float thick, Color color) : Arc(radius, thick, 2.0f * M_PI, color) {}

Ring::~Ring(void) {}





}
