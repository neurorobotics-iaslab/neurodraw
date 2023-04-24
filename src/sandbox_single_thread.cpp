#include <thread>
#include "neurodraw/Rectangle.h"
#include "neurodraw/Circle.h"
#include "neurodraw/Ring.h"
#include "neurodraw/Cross.h"
#include "neurodraw/Arrow.h"
#include "neurodraw/Triangle.h"
#include "neurodraw/Line.h"
#include "neurodraw/Arc.h"
#include "neurodraw/RegularShape.h"
#include "neurodraw/Window.h"
#include "neurodraw/EventKey.h"

neurodraw::Window win("test");
neurodraw::Rectangle rect(0.05f, 0.05f, true, neurodraw::Palette::darkblue);
neurodraw::Circle circle(0.2f);
neurodraw::Ring ring(0.3f, 0.1f);
neurodraw::Cross cross(0.2f, 0.05f);
neurodraw::Arrow arrow(0.2f, 0.1f, true, neurodraw::Palette::darkviolet);
neurodraw::Triangle tri(0.1f, 0.2f, false);
neurodraw::Line line(0.0f, -0.05f, 0.0f, 0.05f, neurodraw::Palette::purple);
neurodraw::Arc arc(0.3f, 0.1f, M_PI / 3.0f, neurodraw::Palette::gold);
neurodraw::RegularShape regular(0.2f, 12, true, neurodraw::Palette::grey);
bool user_quit = false;

void redraw(void) {
	win.clear();
	regular.draw();
	arc.draw();
	line.draw();
	tri.draw();
	arrow.draw();
	cross.draw();
	ring.draw();
	circle.draw();
	rect.draw();
	win.update();
}


void callback_redraw(void) {
	redraw();
}

void callback_quit(void) {
	user_quit = true;
}

void callback_mouse(const neurodraw::MouseEvent& event) {

	unsigned int width, height;
	float min_x, max_x;

	win.size(&width, &height);

	float x = float(event.x)/height;
	float y = float(event.y)/height;

	max_x = float(width)/float(height);

	float py = - (2.0f*y - 1.0f);
	float px = 2.0f*x - max_x;

	rect.move(px, py);
}

void callback_keyboard(const neurodraw::KeyboardEvent& event) {
	if(event.state == 1) {
		printf("Key pressed: %d\n", event.sym);
		 switch(event.sym) {
			 case neurodraw::EventKey::ESCAPE:
				 user_quit = 1;
				 break;
			 case neurodraw::EventKey::a:
				 rect.set_color(neurodraw::Palette::tomato);
				 break;
			 case neurodraw::EventKey::s:
				 rect.set_color(neurodraw::Palette::darkblue);
				 break;
			 case neurodraw::EventKey::d:
				 rect.set_color(neurodraw::Palette::darkgreen);
				 break;
			 case neurodraw::EventKey::z:
				 rect.set_alpha(0.0f);
				 break;
			 case neurodraw::EventKey::x:
				 rect.set_alpha(0.5f);
				 break;
			 case neurodraw::EventKey::c:
				 rect.set_alpha(1.0f);
				 break;
			 case neurodraw::EventKey::l:
				 circle.relrotate(45.0f, 0.0f, 0.0f);
				 break;
			 case neurodraw::EventKey::i:
				 line.relrotate(45.0f, 0.0f, 0.0f);
				 break;
		 }
	}
}

int main(int argc, char** argv) {


	win.on_redraw(callback_redraw);
	win.on_quit(callback_quit);
	win.on_mouse(callback_mouse);
	win.on_keyboard(callback_keyboard);

	win.open();

	ring.move(0.5f, 0.5f);
	ring.set_color(neurodraw::Palette::blue);
	circle.move(-0.5f, -0.5f);
	cross.set_color(neurodraw::Palette::red);
	arrow.move(0.5f, -0.5f);
	tri.set_color(neurodraw::Palette::yellow);
	tri.move(-0.5f, 0.5f);
	line.move(-0.5f, 0.0f);
	arc.move(0.5f, -0.2f);
	regular.move(0.0f, -0.5f);
	
	while(user_quit == false) {
		redraw();
		win.process_events();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	win.close();


	return 0;
}
