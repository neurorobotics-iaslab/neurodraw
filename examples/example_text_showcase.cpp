#include <chrono>
#include <cstdio>
#include <thread>

#include "neurodraw/EventKey.h"
#include "neurodraw/Line.h"
#include "neurodraw/Text.h"
#include "neurodraw/Window.h"

namespace {

neurodraw::Window win("NeuroDraw Text Showcase");
bool user_quit = false;

neurodraw::Line axis_x(2.0f, neurodraw::Palette::dimgray);
neurodraw::Line axis_y(1.4f, neurodraw::Palette::dimgray);
neurodraw::Text title("NeuroDraw Text", "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 0.18f, 56, neurodraw::Palette::white);
neurodraw::Text subtitle("SDL2_ttf + OpenGL texture quad", "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 0.08f, 28, neurodraw::Palette::lightgray);
neurodraw::Text hint("SPACE recolors, A/S alpha, Q/W size, ESC quit", "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 0.07f, 24, neurodraw::Palette::gold);
neurodraw::Text rotating("Rotating label", "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 0.10f, 32, neurodraw::Palette::tomato);
neurodraw::Text status("Move the mouse", "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 0.08f, 26, neurodraw::Palette::deepskyblue);

float phase = 0.0f;
bool alt_palette = false;

void setup_scene() {
	axis_x.set_stroke_alpha(0.18f);
	axis_x.set_stroke_width(1.5f);
	axis_y.rotate(90.0f);
	axis_y.set_stroke_alpha(0.18f);
	axis_y.set_stroke_width(1.5f);

	title.move(0.0f, 0.62f);
	subtitle.move(0.0f, 0.42f);
	hint.move(0.0f, -0.66f);
	rotating.move(0.0f, 0.0f);
	status.move(0.0f, -0.32f);
}

void redraw() {
	win.clear();
	axis_x.draw();
	axis_y.draw();
	title.draw();
	subtitle.draw();
	hint.draw();
	rotating.draw();
	status.draw();
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

	rotating.move(px * 0.5f, py * 0.5f);
}

void callback_keyboard(const neurodraw::KeyboardEvent& event) {
	if(event.state != 1)
		return;

	switch(event.sym) {
		case neurodraw::EventKey::ESCAPE:
			user_quit = true;
			break;

		case neurodraw::EventKey::SPACE:
			alt_palette = !alt_palette;
			if(alt_palette) {
				title.set_color(neurodraw::Palette::gold);
				subtitle.set_color(neurodraw::Palette::white);
				rotating.set_color(neurodraw::Palette::limegreen);
			} else {
				title.set_color(neurodraw::Palette::white);
				subtitle.set_color(neurodraw::Palette::lightgray);
				rotating.set_color(neurodraw::Palette::tomato);
			}
			break;

		case neurodraw::EventKey::a:
			subtitle.set_alpha(0.35f);
			status.set_alpha(0.35f);
			break;

		case neurodraw::EventKey::s:
			subtitle.set_alpha(1.0f);
			status.set_alpha(1.0f);
			break;

		case neurodraw::EventKey::q:
			title.set_height(0.12f);
			break;

		case neurodraw::EventKey::w:
			title.set_height(0.20f);
			break;
	}
}

}

int main(int argc, char** argv) {
	(void) argc;
	(void) argv;

	setup_scene();

	win.on_redraw(callback_redraw);
	win.on_quit(callback_quit);
	win.on_mouse(callback_mouse);
	win.on_keyboard(callback_keyboard);

	if(!win.open())
		return 1;

	std::printf("ESC quit | SPACE palette | A/S alpha | Q/W title size\n");

	while(!user_quit) {
		phase += 0.03f;
		rotating.rotate(phase * 35.0f);
		status.set_text((phase < 6.0f) ? "Move the mouse" : "Text uses SDL2_ttf when available");
		redraw();
		win.process_events();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	win.close();
	return 0;
}
