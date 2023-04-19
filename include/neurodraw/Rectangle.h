#ifndef NEURODRAW_RECTANGLE_
#define NEURODRAW_RECTANGLE_

#include "neurodraw/Shape.h"


namespace neurodraw {

class Rectangle : public Shape {

	public:
		Rectangle(float width, float height, bool is_filled = false, Color color = Palette::white);
		virtual ~Rectangle(void);

};


}

#endif
