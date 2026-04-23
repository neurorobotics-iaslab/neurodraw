#ifndef NEURODRAW_IMAGE_H_
#define NEURODRAW_IMAGE_H_

#include <memory>
#include <string>

#include "neurodraw/Shape.h"

namespace neurodraw {

class Image : public Shape {
	public:
		enum class FitMode {
			Contain,
			Cover,
			Stretch
		};

		enum class HorizontalAlign {
			Left,
			Center,
			Right
		};

		enum class VerticalAlign {
			Top,
			Middle,
			Bottom
		};

		Image(const std::string& path = "", float height = 0.30f);
		virtual ~Image(void);

		bool load(const std::string& path);
		std::string path(void);

		void set_size(float width, float height);
		void set_height(float height);
		float height(void);
		void set_width(float width);
		float width(void);
		float intrinsic_width(void);
		float intrinsic_height(void);

		virtual void set_alpha(float alpha) override;
		virtual float alpha(void) override;
		virtual void hide(void) override;
		virtual void show(void) override;

		void set_fit_mode(FitMode mode);
		FitMode fit_mode(void);

		void set_alignment(HorizontalAlign horizontal, VerticalAlign vertical = VerticalAlign::Middle);
		HorizontalAlign horizontal_alignment(void);
		VerticalAlign vertical_alignment(void);

		virtual void draw(void) override;
		virtual void local_bounds(float* min_x, float* min_y,
		                         float* max_x, float* max_y) override;

	private:
		bool upload_texture(void);

	private:
		class Impl;
		std::unique_ptr<Impl> impl_;
};

}

#endif
