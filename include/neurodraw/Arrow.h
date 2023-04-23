#ifndef NEURODRAW_ARROW_H_
#define NEURODRAW_ARROW_H_

#include "neurodraw/common.h"
#include "neurodraw/Shape.h"

namespace neurodraw {

class Arrow : public Shape {

	public:
		
		Arrow(float width, float height, float headsize, float bodysize, bool is_filled, Color color = Palette::white);
		Arrow(float width, float height, bool is_filled, Color color = Palette::white);
		virtual ~Arrow(void);


	private:
		void create_arrow_filled(float width, float height, float headsize, float bodysize);
		void create_arrow_stroke(float width, float height, float headsize, float bodysize);
};


}


#endif
