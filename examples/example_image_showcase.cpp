#include <chrono>
#include <cstdio>
#include <string>
#include <thread>

#include "neurodraw/EventKey.h"
#include "neurodraw/Image.h"
#include "neurodraw/Line.h"
#include "neurodraw/Text.h"
#include "neurodraw/Window.h"

namespace {

const std::string asset_dir = NEURODRAW_EXAMPLE_ASSET_DIR;
const std::string png_path = asset_dir + "/demo.png";
const std::string jpg_path = asset_dir + "/demo.jpg";
const std::string gif_path = asset_dir + "/demo.gif";

neurodraw::Window win("NeuroDraw Image Showcase");
bool user_quit = false;
float phase = 0.0f;

neurodraw::Line axis_x(2.0f, neurodraw::Palette::dimgray);
neurodraw::Line axis_y(1.4f, neurodraw::Palette::dimgray);
neurodraw::Image logo(png_path, 0.52f);
neurodraw::Text caption("Image class: PNG/JPG/GIF/BMP/PPM via SDL2_image + built-in loaders", "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 0.08f, 28, neurodraw::Palette::white);
neurodraw::Text hint("A/S alpha  Q/W height  E/R width  T set_size  1/2/3 format  C/V/B fit  ESC quit", "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 0.07f, 24, neurodraw::Palette::gold);

void setup_scene() {
	axis_x.set_stroke_alpha(0.18f);
	axis_x.set_stroke_width(1.5f);
	axis_y.rotate(90.0f);
	axis_y.set_stroke_alpha(0.18f);
	axis_y.set_stroke_width(1.5f);

	logo.move(0.0f, 0.10f);
	logo.set_width(0.52f);
	logo.set_fit_mode(neurodraw::Image::FitMode::Contain);
	caption.move(0.0f, -0.38f);
	hint.move(0.0f, -0.62f);
}

void redraw() {
	win.clear();
	axis_x.draw();
	axis_y.draw();
	logo.draw();
	caption.draw();
	hint.draw();
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

	const float max_x = static_cast<float>(width) / static_cast<float>(height);
	const float x = static_cast<float>(event.x) / static_cast<float>(height);
	const float y = static_cast<float>(event.y) / static_cast<float>(height);

	const float py = -(2.0f * y - 1.0f);
	const float px = 2.0f * x - max_x;

	logo.move(px * 0.5f, py * 0.35f + 0.10f);
}

void callback_keyboard(const neurodraw::KeyboardEvent& event) {
	if(event.state != 1)
		return;

	switch(event.sym) {
		case neurodraw::EventKey::ESCAPE:
			user_quit = true;
			break;
		case neurodraw::EventKey::a:
			logo.set_alpha(0.30f);
			break;
		case neurodraw::EventKey::s:
			logo.set_alpha(1.0f);
			break;
		case neurodraw::EventKey::q:
			logo.set_height(0.30f);
			break;
		case neurodraw::EventKey::w:
			logo.set_height(0.65f);
			break;
		case neurodraw::EventKey::e:
			logo.set_width(0.28f);
			break;
		case neurodraw::EventKey::r:
			logo.set_width(0.72f);
			break;
		case neurodraw::EventKey::t:
			logo.set_size(0.72f, 0.34f);
			break;
		case neurodraw::EventKey::NUM_1:
			logo.load(png_path);
			break;
		case neurodraw::EventKey::NUM_2:
			logo.load(jpg_path);
			break;
		case neurodraw::EventKey::NUM_3:
			logo.load(gif_path);
			break;
		case neurodraw::EventKey::c:
			logo.set_fit_mode(neurodraw::Image::FitMode::Contain);
			break;
		case neurodraw::EventKey::v:
			logo.set_fit_mode(neurodraw::Image::FitMode::Cover);
			break;
		case neurodraw::EventKey::b:
			logo.set_fit_mode(neurodraw::Image::FitMode::Stretch);
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

	std::printf("ESC quit | A/S alpha | Q/W height | E/R width | T set_size | 1 png | 2 jpg | 3 gif | C contain | V cover | B stretch\n");

	while(!user_quit) {
		phase += 0.03f;
		logo.rotate(phase * 30.0f);
		redraw();
		win.process_events();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	win.close();
	return 0;
}
