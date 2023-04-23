#ifndef NEURODRAW_CROSS_H_
#define NEURODRAW_CROSS_H_

#include "neurodraw/common.h"
#include "neurodraw/Shape.h"

namespace neurodraw {

class Cross : public Shape {

	public:
		Cross(float size, float thick, Color color = Palette::white);
		virtual ~Cross(void);

};


}


#endif
