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

neurodraw::Window win("NeuroDraw Gallery");
bool user_quit = false;
bool animate = true;
float phase = 0.0f;

neurodraw::Rectangle card_rect(0.42f, 0.30f, true, neurodraw::Palette::steelblue);
neurodraw::Triangle card_tri(0.26f, 0.22f, true, neurodraw::Palette::gold);
neurodraw::RegularShape card_poly(0.14f, 8, true, neurodraw::Palette::mediumseagreen);
neurodraw::Circle card_circle(0.13f, true, neurodraw::Palette::orchid);
neurodraw::Ring card_ring(0.14f, 0.05f, neurodraw::Palette::royalblue);
neurodraw::Arc card_arc(0.14f, 0.05f, 1.35f * static_cast<float>(M_PI), neurodraw::Palette::orange);
neurodraw::Arrow card_arrow(0.28f, 0.16f, true, neurodraw::Palette::tomato);
neurodraw::Cross card_cross(0.22f, 0.05f, neurodraw::Palette::crimson);
neurodraw::Line divider_h(2.4f, neurodraw::Palette::dimgray);
neurodraw::Line divider_v(1.8f, neurodraw::Palette::dimgray);
neurodraw::Circle cursor(0.04f, true, neurodraw::Palette::white);

void setup_shapes() {
	card_rect.move(-0.75f, 0.45f);
	card_rect.set_fill_alpha(0.30f);
	card_rect.set_stroke_color(neurodraw::Palette::white);
	card_rect.set_stroke_width(3.0f);

	card_tri.move(0.0f, 0.45f);
	card_tri.set_fill_alpha(0.90f);
	card_tri.set_stroke_color(neurodraw::Palette::white);
	card_tri.set_stroke_width(3.0f);

	card_poly.move(0.75f, 0.45f);
	card_poly.set_fill_alpha(0.75f);
	card_poly.set_stroke_color(neurodraw::Palette::white);
	card_poly.set_stroke_width(2.5f);
	card_poly.set_color(neurodraw::Palette::greenyellow, 3);

	card_circle.move(-0.75f, -0.10f);
	card_circle.set_fill_alpha(0.70f);
	card_circle.set_stroke_color(neurodraw::Palette::white);
	card_circle.set_stroke_width(3.0f);

	card_ring.move(0.0f, -0.10f);
	card_ring.set_fill_alpha(0.55f);
	card_ring.set_stroke_color(neurodraw::Palette::aliceblue);
	card_ring.set_stroke_width(2.0f);

	card_arc.move(0.75f, -0.10f);
	card_arc.set_fill_alpha(0.85f);
	card_arc.set_stroke_color(neurodraw::Palette::white);
	card_arc.set_stroke_width(2.0f);

	card_arrow.move(-0.75f, -0.65f);
	card_arrow.set_stroke_color(neurodraw::Palette::black);
	card_arrow.set_stroke_width(3.0f);

	card_cross.move(0.0f, -0.65f);
	card_cross.set_fill_alpha(0.95f);
	card_cross.set_stroke_color(neurodraw::Palette::white);
	card_cross.set_stroke_width(2.5f);

	divider_h.move(0.0f, 0.18f);
	divider_h.set_stroke_alpha(0.18f);
	divider_h.set_stroke_width(1.5f);

	divider_v.move(0.0f, -0.10f);
	divider_v.rotate(90.0f);
	divider_v.set_stroke_alpha(0.18f);
	divider_v.set_stroke_width(1.5f);

	cursor.set_fill_alpha(0.85f);
	cursor.set_stroke_color(neurodraw::Palette::black);
	cursor.set_stroke_width(2.0f);
}

void animate_shapes() {
	if(!animate)
		return;

	phase += 0.025f;

	card_tri.rotate(std::sin(phase) * 20.0f);
	card_poly.rotate(-phase * 45.0f);
	card_circle.set_fill_alpha(0.35f + 0.45f * (0.5f + 0.5f * std::sin(phase * 1.6f)));
	card_ring.rotate(phase * 35.0f);
	card_ring.set_fill_alpha(0.25f + 0.55f * (0.5f + 0.5f * std::cos(phase * 1.3f)));
	card_arc.rotate(-phase * 28.0f);
	card_arrow.rotate(std::sin(phase * 1.2f) * 30.0f);
	card_cross.rotate(std::cos(phase * 0.8f) * 12.0f);
}

void redraw() {
	win.clear();
	divider_h.draw();
	divider_v.draw();
	card_rect.draw();
	card_tri.draw();
	card_poly.draw();
	card_circle.draw();
	card_ring.draw();
	card_arc.draw();
	card_arrow.draw();
	card_cross.draw();
	cursor.draw();
	win.update();
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

		case neurodraw::EventKey::a:
			card_rect.set_fill_color(neurodraw::Palette::darkslateblue);
			card_circle.set_fill_color(neurodraw::Palette::hotpink);
			card_arrow.set_fill_color(neurodraw::Palette::orange);
			break;

		case neurodraw::EventKey::s:
			card_rect.set_fill_color(neurodraw::Palette::steelblue);
			card_circle.set_fill_color(neurodraw::Palette::orchid);
			card_arrow.set_fill_color(neurodraw::Palette::tomato);
			break;

		case neurodraw::EventKey::d:
			card_rect.set_stroke_width(1.0f);
			card_tri.set_stroke_width(1.0f);
			card_poly.set_stroke_width(1.0f);
			card_circle.set_stroke_width(1.0f);
			card_arrow.set_stroke_width(1.0f);
			break;

		case neurodraw::EventKey::f:
			card_rect.set_stroke_width(4.0f);
			card_tri.set_stroke_width(4.0f);
			card_poly.set_stroke_width(3.0f);
			card_circle.set_stroke_width(4.0f);
			card_arrow.set_stroke_width(4.0f);
			break;

		case neurodraw::EventKey::z:
			card_ring.set_fill_alpha(0.10f);
			card_arc.set_fill_alpha(0.20f);
			break;

		case neurodraw::EventKey::x:
			card_ring.set_fill_alpha(0.70f);
			card_arc.set_fill_alpha(0.85f);
			break;
	}
}

}

int main(int argc, char** argv) {
	(void) argc;
	(void) argv;

	setup_shapes();

	win.on_redraw(callback_redraw);
	win.on_quit(callback_quit);
	win.on_mouse(callback_mouse);
	win.on_keyboard(callback_keyboard);

	if(!win.open())
		return 1;

	std::printf("ESC: quit | SPACE: pause | a/s: palette | d/f: stroke width | z/x: alpha\n");

	while(!user_quit) {
		animate_shapes();
		redraw();
		win.process_events();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	win.close();
	return 0;
}
