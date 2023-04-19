#ifndef NEURODRAW_RECTANGLE_
#define NEURODRAW_RECTANGLE_

#include "neurodraw/Shape.h"


namespace neurodraw {

class Rectangle : public Shape {

	public:
		Rectangle(float width, float height, Color color = Palette::white, bool is_filled = false);
		virtual ~Rectangle(void);

};


}

#endif
