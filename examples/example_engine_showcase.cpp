#include <chrono>
#include <cmath>
#include <cstdio>
#include <thread>

#include "neurodraw/Arc.h"
#include "neurodraw/Arrow.h"
#include "neurodraw/Circle.h"
#include "neurodraw/Engine.h"
#include "neurodraw/EventKey.h"
#include "neurodraw/Line.h"
#include "neurodraw/Rectangle.h"
#include "neurodraw/RegularShape.h"

namespace {

neurodraw::Engine* engine_ptr = nullptr;
bool animate = true;

neurodraw::Rectangle hud(0.52f, 0.24f, true, neurodraw::Palette::midnightblue);
neurodraw::Arrow target(0.30f, 0.18f, true, neurodraw::Palette::darkorange);
neurodraw::RegularShape orbit(0.12f, 6, true, neurodraw::Palette::deepskyblue);
neurodraw::Arc gauge(0.26f, 0.07f, 1.35f * static_cast<float>(M_PI), neurodraw::Palette::limegreen);
neurodraw::Circle cursor(0.05f, true, neurodraw::Palette::white);
neurodraw::Line axis(0.90f, neurodraw::Palette::dimgray);

float phase = 0.0f;

void setup_scene() {
	hud.move(-0.72f, 0.62f);
	hud.set_fill_alpha(0.25f);
	hud.set_stroke_color(neurodraw::Palette::white);
	hud.set_stroke_width(3.0f);

	target.move(0.35f, 0.15f);
	target.set_fill_color(neurodraw::Palette::darkorange);
	target.set_stroke_color(neurodraw::Palette::white);
	target.set_stroke_width(3.0f);

	orbit.move(-0.35f, -0.10f);
	orbit.set_fill_alpha(0.85f);
	orbit.set_stroke_color(neurodraw::Palette::white);
	orbit.set_stroke_width(2.0f);

	gauge.move(0.0f, -0.42f);
	gauge.set_fill_alpha(0.65f);
	gauge.set_stroke_color(neurodraw::Palette::white);
	gauge.set_stroke_width(2.0f);

	cursor.set_fill_alpha(0.90f);
	cursor.set_stroke_color(neurodraw::Palette::black);
	cursor.set_stroke_width(2.0f);

	axis.set_stroke_alpha(0.20f);
	axis.set_stroke_width(1.5f);
}

void animate_scene() {
	if(!animate)
		return;

	phase += 0.025f;

	target.rotate(std::sin(phase) * 40.0f);
	orbit.relrotate(2.0f);
	gauge.rotate(-phase * 35.0f);

	float x = -0.35f + 0.18f * std::cos(phase * 1.7f);
	float y = -0.10f + 0.18f * std::sin(phase * 1.7f);
	orbit.move(x, y);

	float alpha = 0.25f + 0.55f * (0.5f + 0.5f * std::sin(phase * 1.2f));
	gauge.set_fill_alpha(alpha);
}

void callback_mouse(const neurodraw::MouseEvent& event) {
	unsigned int width, height;
	engine_ptr->winsize(&width, &height);

	float max_x = static_cast<float>(width) / static_cast<float>(height);
	float x = static_cast<float>(event.x) / static_cast<float>(height);
	float y = static_cast<float>(event.y) / static_cast<float>(height);

	float py = -(2.0f * y - 1.0f);
	float px = 2.0f * x - max_x;

	cursor.move(px, py);
	target.rotate(std::atan2(py - 0.15f, px - 0.35f) * 180.0f / static_cast<float>(M_PI));
}

void callback_keyboard(const neurodraw::KeyboardEvent& event) {
	if(event.state != 1)
		return;

	switch(event.sym) {
		case neurodraw::EventKey::ESCAPE:
			engine_ptr->quit();
			break;

		case neurodraw::EventKey::SPACE:
			animate = !animate;
			break;

		case neurodraw::EventKey::a:
			target.set_fill_color(neurodraw::Palette::tomato);
			gauge.set_fill_color(neurodraw::Palette::gold);
			break;

		case neurodraw::EventKey::s:
			target.set_fill_color(neurodraw::Palette::darkorange);
			gauge.set_fill_color(neurodraw::Palette::limegreen);
			break;

		case neurodraw::EventKey::d:
			target.set_stroke_width(1.0f);
			orbit.set_stroke_width(1.0f);
			break;

		case neurodraw::EventKey::f:
			target.set_stroke_width(5.0f);
			orbit.set_stroke_width(4.0f);
			break;

		case neurodraw::EventKey::z:
			hud.set_fill_alpha(0.10f);
			break;

		case neurodraw::EventKey::x:
			hud.set_fill_alpha(0.30f);
			break;
	}
}

}

int main(int argc, char** argv) {
	(void) argc;
	(void) argv;

	neurodraw::Engine engine("NeuroDraw Engine Showcase", 60.0f);
	engine_ptr = &engine;

	setup_scene();

	engine.on_keyboard(callback_keyboard);
	engine.on_mouse(callback_mouse);

	engine.add(&hud);
	engine.add(&axis);
	engine.add(&target);
	engine.add(&orbit);
	engine.add(&gauge);
	engine.add(&cursor);

	std::printf("ESC: quit | SPACE: pause | a/s: fill palette | d/f: stroke width | z/x: HUD alpha\n");

	while(engine.is_running()) {
		animate_scene();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	return 0;
}
