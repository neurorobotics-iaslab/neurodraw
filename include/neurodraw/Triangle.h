#ifndef NEURODRAW_TRIANGLE_H_
#define NEURODRAW_TRIANGLE_H_

#include "neurodraw/common.h"
#include "neurodraw/Shape.h"

namespace neurodraw {

class Triangle : public Shape {

	public:
		Triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool is_full, Color color = Palette::white);
		Triangle(float width, float height, bool is_full, Color color = Palette::white);
		virtual ~Triangle(void);

};

}




#endif
