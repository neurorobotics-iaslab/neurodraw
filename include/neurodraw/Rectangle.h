#ifndef NEURODRAW_RECTANGLE_H_
#define NEURODRAW_RECTANGLE_H_

#include "Shape.h"

namespace neurodraw {


class Rectangle : public Shape {

	public:
		Rectangle(float width, float height, bool is_filled = false, Color color = Palette::white);
		virtual ~Rectangle(void);


};

}


#endif
