#ifndef NEURODRAW_REGULAR_SHAPE_
#define NEURODRAW_REGULAR_SHAPE_

#include <cmath>
#include "neurodraw/common.h"
#include "neurodraw/Shape.h"

namespace neurodraw {

class RegularShape : public Shape {

	public:
		RegularShape(float radius, unsigned int nvertices, bool is_filled = false, Color color = Palette::white);
		virtual ~RegularShape(void);

	private:
		void create_regular_shape(float radius, float cx, float cy, Color color, 
						   		  bool is_filled, unsigned int numpoints);


};

}



#endif
