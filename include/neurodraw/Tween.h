#ifndef NEURODRAW_TWEEN_H_
#define NEURODRAW_TWEEN_H_

#include <cstddef>
#include <list>

#include "neurodraw/Shape.h"

namespace neurodraw {

class Tween {

	public:
		enum class Property {
			X,
			Y,
			Rotation,
			Alpha
		};

		enum class Easing {
			Linear,
			EaseIn,
			EaseOut,
			EaseInOut
		};

		Tween(Shape* target = nullptr,
		      Property property = Property::X,
		      float to = 0.0f,
		      float duration = 1.0f,
		      Easing easing = Easing::Linear);

		void bind(Shape* target);
		Shape* target(void) const;

		void set_property(Property property);
		Property property(void) const;

		void set_from(float from);
		void use_current(void);
		void set_to(float to);
		float to(void) const;

		void set_duration(float duration);
		float duration(void) const;

		void set_easing(Easing easing);
		Easing easing(void) const;

		void reset(void);
		bool advance(float dt);
		bool finished(void) const;

	private:
		float current_value(void) const;
		void apply(float value);
		float eased(float t) const;

	private:
		Shape* target_;
		Property property_;
		float from_;
		float to_;
		float duration_;
		float elapsed_;
		Easing easing_;
		bool capture_current_;
		bool finished_;
};

class Animator {

	public:
		Animator(void);

		void add(const Tween& tween);
		void clear(void);
		std::size_t size(void) const;
		void update(float dt);

	private:
		std::list<Tween> tweens_;
};

}

#endif
