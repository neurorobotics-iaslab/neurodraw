#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include "neurodraw/Group.h"
#include "neurodraw/Image.h"
#include "neurodraw/Rectangle.h"
#include "neurodraw/Text.h"
#include "neurodraw/Tween.h"

namespace {

bool check(bool condition, const std::string& message) {
	if(!condition) {
		std::cerr << "[FAIL] " << message << "\n";
		return false;
	}
	return true;
}

bool check_near(float actual, float expected, float tolerance, const std::string& message) {
	if(std::fabs(actual - expected) > tolerance) {
		std::cerr << "[FAIL] " << message << " (expected " << expected
		          << ", got " << actual << ")\n";
		return false;
	}
	return true;
}

}

int main() {
	bool ok = true;

	neurodraw::Rectangle rect(2.0f, 4.0f, true, neurodraw::Palette::white);
	float min_x = 0.0f;
	float min_y = 0.0f;
	float max_x = 0.0f;
	float max_y = 0.0f;
	rect.bounds(&min_x, &min_y, &max_x, &max_y);
	ok &= check_near(min_x, -1.0f, 0.001f, "Rectangle min x before move");
	ok &= check_near(min_y, -2.0f, 0.001f, "Rectangle min y before move");
	ok &= check_near(max_x, 1.0f, 0.001f, "Rectangle max x before move");
	ok &= check_near(max_y, 2.0f, 0.001f, "Rectangle max y before move");

	rect.move(3.0f, -1.0f);
	rect.rotate(90.0f);
	rect.bounds(&min_x, &min_y, &max_x, &max_y);
	ok &= check_near(min_x, 1.0f, 0.02f, "Rectangle min x after rotate");
	ok &= check_near(min_y, -2.0f, 0.02f, "Rectangle min y after rotate");
	ok &= check_near(max_x, 5.0f, 0.02f, "Rectangle max x after rotate");
	ok &= check_near(max_y, 0.0f, 0.02f, "Rectangle max y after rotate");
	ok &= check(rect.contains(3.0f, -1.0f), "Rectangle contains its center after rotation");
	ok &= check(!rect.contains(5.5f, -1.0f), "Rectangle excludes outside point");

	neurodraw::Rectangle lower(0.4f, 0.4f, true, neurodraw::Palette::white);
	neurodraw::Rectangle upper(0.4f, 0.4f, true, neurodraw::Palette::white);
	lower.move(0.0f, 0.0f);
	upper.move(0.0f, 0.0f);
	neurodraw::Group group;
	group.add(&lower);
	group.add(&upper);
	ok &= check(group.hit(0.0f, 0.0f) == &upper, "Group hit returns topmost child");
	group.move(0.5f, -0.25f);
	ok &= check(group.hit(0.5f, -0.25f) == &upper, "Group hit accounts for group transform");

	neurodraw::Tween tween_x(&rect, neurodraw::Tween::Property::X, 10.0f, 1.0f,
	                         neurodraw::Tween::Easing::Linear);
	tween_x.advance(0.5f);
	ok &= check_near(rect.x(), 6.5f, 0.02f, "Tween updates x halfway");
	tween_x.advance(0.5f);
	ok &= check_near(rect.x(), 10.0f, 0.02f, "Tween reaches final x");

	neurodraw::Tween tween_alpha(&rect, neurodraw::Tween::Property::Alpha, 0.2f, 0.5f,
	                             neurodraw::Tween::Easing::EaseOut);
	tween_alpha.advance(0.5f);
	ok &= check_near(rect.alpha(), 0.2f, 0.02f, "Tween updates alpha");

	const std::string font_path = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
	neurodraw::Text text("NeuroDraw", font_path, 0.2f, 28, neurodraw::Palette::white);
	text.set_alignment(neurodraw::Text::HorizontalAlign::Center,
	                   neurodraw::Text::VerticalAlign::Middle);
	ok &= check(text.width() > 0.0f, "Text width is measurable without draw");
	text.local_bounds(&min_x, &min_y, &max_x, &max_y);
	ok &= check_near(max_y - min_y, 0.2f, 0.02f, "Text bounds use requested height");
	ok &= check(text.alpha() == 1.0f, "Text default alpha is 1");
	text.hide();
	ok &= check_near(text.alpha(), 0.0f, 0.001f, "Text hide updates alpha");
	text.show();
	ok &= check_near(text.alpha(), 1.0f, 0.001f, "Text show updates alpha");

	neurodraw::Image image(NEURODRAW_EXAMPLE_ASSET_DIR "/demo.png", 0.25f);
	ok &= check(image.intrinsic_width() > 0.0f, "Image loads intrinsic width");
	ok &= check(image.intrinsic_height() > 0.0f, "Image loads intrinsic height");
	image.set_size(0.60f, 0.20f);
	image.set_fit_mode(neurodraw::Image::FitMode::Contain);
	image.local_bounds(&min_x, &min_y, &max_x, &max_y);
	ok &= check((max_x - min_x) <= 0.60f + 0.001f, "Image contain width stays in box");
	ok &= check((max_y - min_y) <= 0.20f + 0.001f, "Image contain height stays in box");
	image.set_fit_mode(neurodraw::Image::FitMode::Cover);
	image.local_bounds(&min_x, &min_y, &max_x, &max_y);
	ok &= check_near(max_x - min_x, 0.60f, 0.001f, "Image cover uses full box width");
	ok &= check_near(max_y - min_y, 0.20f, 0.001f, "Image cover uses full box height");

	return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
