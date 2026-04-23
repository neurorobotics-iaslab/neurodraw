#include <chrono>
#include <cmath>
#include <cstdio>
#include <thread>

#include "neurodraw/Arc.h"
#include "neurodraw/Arrow.h"
#include "neurodraw/Circle.h"
#include "neurodraw/Cross.h"
#include "neurodraw/EventKey.h"
#include "neurodraw/Line.h"
#include "neurodraw/Rectangle.h"
#include "neurodraw/RegularShape.h"
#include "neurodraw/Ring.h"
#include "neurodraw/Triangle.h"
#include "neurodraw/Window.h"

namespace {

neurodraw::Window win("NeuroDraw Feature Showcase");
bool user_quit = false;
bool animate = true;

neurodraw::Rectangle panel(0.55f, 0.32f, true, neurodraw::Palette::steelblue);
neurodraw::Triangle triangle(0.26f, 0.24f, true, neurodraw::Palette::gold);
neurodraw::RegularShape polygon(0.18f, 7, true, neurodraw::Palette::mediumseagreen);
neurodraw::Arrow arrow(0.34f, 0.20f, true, neurodraw::Palette::tomato);
neurodraw::Arc arc(0.22f, 0.06f, 1.5f * static_cast<float>(M_PI), neurodraw::Palette::darkorange);
neurodraw::Ring ring(0.20f, 0.06f, neurodraw::Palette::royalblue);
neurodraw::Cross cross(0.22f, 0.05f, neurodraw::Palette::crimson);
neurodraw::Circle cursor(0.06f, true, neurodraw::Palette::white);
neurodraw::Line guide_x(0.60f, neurodraw::Palette::lightgray);
neurodraw::Line guide_y(0.60f, neurodraw::Palette::lightgray);

float phase = 0.0f;

void style_scene() {
	panel.move(-0.78f, 0.58f);
	panel.set_fill_color(neurodraw::Palette::steelblue);
	panel.set_fill_alpha(0.35f);
	panel.set_stroke_color(neurodraw::Palette::white);
	panel.set_stroke_width(3.0f);

	triangle.move(-0.65f, -0.18f);
	triangle.set_fill_color(neurodraw::Palette::gold);
	triangle.set_stroke_color(neurodraw::Palette::white);
	triangle.set_fill_alpha(0.90f);
	triangle.set_stroke_alpha(1.0f);
	triangle.set_stroke_width(3.0f);

	polygon.move(0.00f, 0.28f);
	polygon.set_fill_color(neurodraw::Palette::mediumseagreen);
	polygon.set_stroke_color(neurodraw::Palette::white);
	polygon.set_stroke_width(2.5f);
	polygon.set_fill_alpha(0.65f);
	polygon.set_color(neurodraw::Palette::greenyellow, 2);

	arrow.move(0.62f, 0.38f);
	arrow.set_fill_color(neurodraw::Palette::tomato);
	arrow.set_stroke_color(neurodraw::Palette::black);
	arrow.set_stroke_width(3.0f);

	arc.move(0.60f, -0.12f);
	arc.set_fill_color(neurodraw::Palette::orange);
	arc.set_stroke_color(neurodraw::Palette::white);
	arc.set_fill_alpha(0.80f);
	arc.set_stroke_width(2.0f);

	ring.move(-0.05f, -0.45f);
	ring.set_fill_color(neurodraw::Palette::royalblue);
	ring.set_stroke_color(neurodraw::Palette::aliceblue);
	ring.set_fill_alpha(0.55f);
	ring.set_stroke_width(2.0f);

	cross.move(-0.78f, 0.58f);
	cross.set_fill_alpha(0.90f);
	cross.set_stroke_color(neurodraw::Palette::white);
	cross.set_stroke_width(2.5f);

	cursor.move(0.0f, 0.0f);
	cursor.set_fill_color(neurodraw::Palette::white);
	cursor.set_stroke_color(neurodraw::Palette::black);
	cursor.set_fill_alpha(0.85f);
	cursor.set_stroke_width(2.0f);

	guide_x.move(0.0f, 0.0f);
	guide_x.set_stroke_alpha(0.25f);
	guide_x.set_stroke_width(1.5f);

	guide_y.move(0.0f, 0.0f);
	guide_y.rotate(90.0f);
	guide_y.set_stroke_alpha(0.25f);
	guide_y.set_stroke_width(1.5f);
}

void redraw() {
	win.clear();
	panel.draw();
	cross.draw();
	guide_x.draw();
	guide_y.draw();
	triangle.draw();
	polygon.draw();
	arrow.draw();
	arc.draw();
	ring.draw();
	cursor.draw();
	win.update();
}

void update_animation() {
	if(!animate)
		return;

	phase += 0.03f;

	triangle.rotate(std::sin(phase) * 35.0f);
	polygon.rotate(-phase * 45.0f);
	arrow.rotate(std::cos(phase * 0.8f) * 20.0f);
	ring.rotate(phase * 30.0f);
	arc.rotate(-phase * 35.0f);

	float pulse = 0.45f + 0.35f * (0.5f + 0.5f * std::sin(phase * 1.4f));
	ring.set_fill_alpha(pulse);
	panel.set_fill_alpha(0.20f + 0.15f * (0.5f + 0.5f * std::cos(phase)));
}

void callback_redraw() {
	redraw();
}

void callback_quit() {
	user_quit = true;
}

void callback_mouse(const neurodraw::MouseEvent& event) {
	unsigned int width, height;
	win.size(&width, &height);

	float max_x = static_cast<float>(width) / static_cast<float>(height);
	float x = static_cast<float>(event.x) / static_cast<float>(height);
	float y = static_cast<float>(event.y) / static_cast<float>(height);

	float py = -(2.0f * y - 1.0f);
	float px = 2.0f * x - max_x;

	cursor.move(px, py);
	arrow.rotate(std::atan2(py - 0.38f, px - 0.62f) * 180.0f / static_cast<float>(M_PI));
}

void callback_keyboard(const neurodraw::KeyboardEvent& event) {
	if(event.state != 1)
		return;

	switch(event.sym) {
		case neurodraw::EventKey::ESCAPE:
			user_quit = true;
			break;

		case neurodraw::EventKey::SPACE:
			animate = !animate;
			break;

		case neurodraw::EventKey::f:
			triangle.set_fill_color(neurodraw::Palette::tomato);
			polygon.set_fill_color(neurodraw::Palette::gold);
			break;

		case neurodraw::EventKey::g:
			triangle.set_fill_color(neurodraw::Palette::gold);
			polygon.set_fill_color(neurodraw::Palette::mediumseagreen);
			break;

		case neurodraw::EventKey::o:
			triangle.set_stroke_color(neurodraw::Palette::black);
			polygon.set_stroke_color(neurodraw::Palette::black);
			arrow.set_stroke_color(neurodraw::Palette::black);
			break;

		case neurodraw::EventKey::p:
			triangle.set_stroke_color(neurodraw::Palette::white);
			polygon.set_stroke_color(neurodraw::Palette::white);
			arrow.set_stroke_color(neurodraw::Palette::white);
			break;

		case neurodraw::EventKey::z:
			ring.set_stroke_width(1.0f);
			arrow.set_stroke_width(1.0f);
			break;

		case neurodraw::EventKey::x:
			ring.set_stroke_width(4.0f);
			arrow.set_stroke_width(4.0f);
			break;

		case neurodraw::EventKey::c:
			ring.set_fill_alpha(0.15f);
			arc.set_fill_alpha(0.25f);
			break;

		case neurodraw::EventKey::v:
			ring.set_fill_alpha(0.75f);
			arc.set_fill_alpha(0.80f);
			break;
	}
}

}

int main(int argc, char** argv) {
	(void) argc;
	(void) argv;

	style_scene();

	win.on_redraw(callback_redraw);
	win.on_quit(callback_quit);
	win.on_mouse(callback_mouse);
	win.on_keyboard(callback_keyboard);

	if(!win.open())
		return 1;

	std::printf("ESC: quit | SPACE: pause animation | f/g: change fill | o/p: change stroke color | z/x: stroke width | c/v: alpha\n");

	while(!user_quit) {
		update_animation();
		redraw();
		win.process_events();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	win.close();
	return 0;
}
