#ifndef NEURODRAW_LAYER_H_
#define NEURODRAW_LAYER_H_

#include "neurodraw/Group.h"

namespace neurodraw {

class Layer : public Group {

	public:
		Layer(void);
		virtual ~Layer(void);
};

}

#endif
