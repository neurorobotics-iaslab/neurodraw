#include "neurodraw/Rectangle.h"
#include "neurochrono/Rate.h"

int main() {
	neurodraw::Rectangle rect(0.5f, 0.3f, true, neurodraw::Palette::white);
	rect.move(0.1f, -0.2f);

	neurochrono::Rate rate(60.0f);
	rate.sleep();

	return rect.contains(0.1f, -0.2f) ? 0 : 1;
}
