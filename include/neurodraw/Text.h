#ifndef NEURODRAW_TEXT_H_
#define NEURODRAW_TEXT_H_

#include <memory>
#include <string>

#include "neurodraw/Shape.h"

namespace neurodraw {

class Text : public Shape {
	public:
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

		Text(const std::string& text = "",
		     const std::string& font_path = "",
		     float height = 0.15f,
		     int font_size = 48,
		     Color color = Palette::white);
		virtual ~Text(void);

		void set_text(const std::string& text);
		std::string text(void);

		void set_font(const std::string& font_path, int font_size = 48);
		std::string font(void);
		int font_size(void);

		void set_height(float height);
		float height(void);
		float width(void);

		void set_color(Color color);
		virtual void set_alpha(float alpha) override;
		virtual float alpha(void) override;
		virtual void hide(void) override;
		virtual void show(void) override;

		void set_alignment(HorizontalAlign horizontal, VerticalAlign vertical = VerticalAlign::Middle);
		HorizontalAlign horizontal_alignment(void);
		VerticalAlign vertical_alignment(void);

		void set_wrap_width(int pixels);
		int wrap_width(void);

		void set_line_spacing(float multiplier);
		float line_spacing(void);

		virtual void draw(void) override;
		virtual void local_bounds(float* min_x, float* min_y,
		                         float* max_x, float* max_y) override;

	private:
		class Impl;
		std::unique_ptr<Impl> impl_;
};

}

#endif
