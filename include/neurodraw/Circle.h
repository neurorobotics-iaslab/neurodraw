#ifndef NEURODRAW_CIRCLE_H_
#define NEURODRAW_CIRCLE_H_

#include "neurodraw/common.h"
#include "neurodraw/RegularShape.h"

namespace neurodraw {

class Circle : public RegularShape {
	
	public:
		Circle(float radius, bool is_filled = false, Color color = Palette::white);
		virtual ~Circle(void);

};


}


#endif
