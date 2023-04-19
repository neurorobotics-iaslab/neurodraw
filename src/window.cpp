#include <iostream>
#include <drawtk.h>
#include <dtk_event.h>

#include "neurodraw/neurodraw.h"

neurodraw::Display display;
neurodraw::Rectangle rect(0.5, 0.1);
int user_quit = 0;
bool visibility = true;

void callback_quit(void) {
	user_quit = 1;
}

void callback_keyboard(neurodraw::EventKeyboard key) {

	switch(key.sym) {
		case neurodraw::EventKey::ESCAPE:
			user_quit = 1;
			break;
		case neurodraw::EventKey::a:
			rect.set_color(neurodraw::Palette::red);
			break;
		case neurodraw::EventKey::s:
			rect.set_color(neurodraw::Palette::blue);
			break;
		case neurodraw::EventKey::d:
			rect.set_color(neurodraw::Palette::yellow);
			break;
		default:
			break;
	}
}

void callback_mousemotion(neurodraw::EventMouse mouse) {
	unsigned int width, height;
	float min_x, max_x;
	
	display.size(&width, &height);

	float x = float(mouse.x)/height;
	float y = float(mouse.y)/height;

	max_x = float(width)/float(height);

	float py = - (2.0f*y - 1.0f);
	float px = 2.0f*x - max_x;

	rect.move(px, py);
}

void callback_mousebutton(neurodraw::EventMouse mouse) {

	if(mouse.state == 1)
		return;

	if(visibility == true) {
		rect.show();
		visibility = false;
	} else {
		rect.hide();
		visibility = true;
	}

}

int main(int argc, char** argv) {

	
	neurodraw::EventHandler eventhandler;

	eventhandler.on_quit        = callback_quit;
	eventhandler.on_keyboard 	= callback_keyboard;
	eventhandler.on_mousemotion = callback_mousemotion;
	eventhandler.on_mousebutton = callback_mousebutton;

	display.draw(&rect);


	while (user_quit == false) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	}
	
	display.quit();

	return 0;

}
