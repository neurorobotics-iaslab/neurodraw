#ifndef NEURODRAW_ARC_H_
#define NEURODRAW_ARC_H_

#include <cmath>
#include "neurodraw/common.h"
#include "neurodraw/Shape.h"


namespace neurodraw {

class Arc : public Shape{

	public:
		Arc(float radius, float thick, float angle, Color color = Palette::white);
		virtual ~Arc(void);
	
	protected:
		void create_arc(float radius, float thick, float angle, float cx, float cy, Color color, 
						 unsigned int numpoints);

};

}


#endif
