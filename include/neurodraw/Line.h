#ifndef NEURODRAW_LINE_H_
#define NEURODRAW_LINE_H_

#include "neurodraw/common.h"
#include "neurodraw/Shape.h"

namespace neurodraw {

class Line : public Shape {

	public:
		Line(float x1, float y1, float x2, float y2, Color color = Palette::white);
		Line(float length, Color color = Palette::white);
		virtual ~Line(void);

};


}
#endif
