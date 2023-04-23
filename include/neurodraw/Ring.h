#ifndef NEURODRAW_RING_H_
#define NEURODRAW_RING_H_

#include "neurodraw/Arc.h"

namespace neurodraw {

class Ring : public Arc {
	
	public:
		Ring(float radius, float thick, Color color = Palette::white);
		virtual ~Ring(void);

};


}


#endif
