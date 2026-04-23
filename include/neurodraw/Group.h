#ifndef NEURODRAW_GROUP_H_
#define NEURODRAW_GROUP_H_

#include <cstddef>
#include <mutex>
#include <vector>

#include "neurodraw/Shape.h"

namespace neurodraw {

class Group : public Shape {

	public:
		Group(void);
		virtual ~Group(void);

		bool add(Shape* shape);
		bool erase(unsigned int index);
		void clear(void);
		std::size_t size(void);
		Shape* at(unsigned int index);

		virtual void draw(void) override;
		virtual void local_bounds(float* min_x, float* min_y,
		                         float* max_x, float* max_y) override;
		virtual bool contains(float x, float y) override;
		virtual Shape* hit(float x, float y) override;

	protected:
		void lock_children(void);
		void unlock_children(void);

	private:
		std::vector<Shape*> children_;
		std::mutex children_mutex_;
};

}

#endif
