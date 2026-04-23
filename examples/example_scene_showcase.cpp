#include <cstdio>

#include "neurodraw/Circle.h"
#include "neurodraw/Engine.h"
#include "neurodraw/EventKey.h"
#include "neurodraw/Group.h"
#include "neurodraw/Layer.h"
#include "neurodraw/Rectangle.h"
#include "neurodraw/Text.h"
#include "neurodraw/Triangle.h"

namespace {

neurodraw::Engine* engine_ptr = nullptr;

neurodraw::Layer background_layer;
neurodraw::Layer scene_layer;
neurodraw::Layer overlay_layer;

neurodraw::Rectangle background_card(2.40f, 1.55f, true, neurodraw::Palette::midnightblue);
neurodraw::Rectangle panel(1.15f, 0.78f, true, neurodraw::Palette::slateblue);
neurodraw::Circle beacon(0.16f, true, neurodraw::Palette::gold);
neurodraw::Triangle pointer(0.24f, 0.20f, true, neurodraw::Palette::tomato);
neurodraw::Group cluster;

neurodraw::Circle cursor(0.03f, true, neurodraw::Palette::white);
neurodraw::Rectangle hover_frame(0.44f, 0.32f, false, neurodraw::Palette::aquamarine);
neurodraw::Text status("Hover a shape", "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 0.11f, 30, neurodraw::Palette::white);

float cluster_side = -0.40f;

void place_hover_frame(neurodraw::Shape* shape) {
	float min_x = 0.0f;
	float min_y = 0.0f;
	float max_x = 0.0f;
	float max_y = 0.0f;
	shape->bounds(&min_x, &min_y, &max_x, &max_y);
	hover_frame.move((min_x + max_x) * 0.5f, (min_y + max_y) * 0.5f);
}

void reset_strokes() {
	panel.set_stroke_color(neurodraw::Palette::white);
	panel.set_stroke_width(3.0f);
	beacon.set_stroke_color(neurodraw::Palette::white);
	beacon.set_stroke_width(2.0f);
	pointer.set_stroke_color(neurodraw::Palette::white);
	pointer.set_stroke_width(2.0f);
}

void setup_scene() {
	background_card.set_fill_alpha(0.25f);
	background_card.move(0.0f, 0.0f);

	panel.move(0.0f, 0.0f);
	panel.set_fill_alpha(0.35f);
	panel.set_stroke_color(neurodraw::Palette::white);
	panel.set_stroke_width(3.0f);

	beacon.move(0.24f, 0.05f);
	beacon.set_fill_alpha(0.95f);
	beacon.set_stroke_color(neurodraw::Palette::white);
	beacon.set_stroke_width(2.0f);

	pointer.move(-0.24f, -0.05f);
	pointer.rotate(-15.0f);
	pointer.set_fill_alpha(0.90f);
	pointer.set_stroke_color(neurodraw::Palette::white);
	pointer.set_stroke_width(2.0f);

	cluster.add(&panel);
	cluster.add(&beacon);
	cluster.add(&pointer);
	cluster.move(cluster_side, 0.05f);

	status.move(0.0f, 0.86f);
	status.set_alignment(neurodraw::Text::HorizontalAlign::Center,
	                     neurodraw::Text::VerticalAlign::Middle);

	cursor.set_fill_alpha(0.95f);
	cursor.set_stroke_color(neurodraw::Palette::black);
	cursor.set_stroke_width(1.0f);

	hover_frame.set_stroke_color(neurodraw::Palette::aquamarine);
	hover_frame.set_stroke_width(3.0f);
	hover_frame.set_alpha(0.0f);

	background_layer.add(&background_card);
	scene_layer.add(&cluster);
	overlay_layer.add(&cursor);
	overlay_layer.add(&hover_frame);
	overlay_layer.add(&status);
}

void queue_cluster_animation() {
	cluster_side = -cluster_side;

	engine_ptr->add_tween(neurodraw::Tween(&cluster,
	                                       neurodraw::Tween::Property::X,
	                                       cluster_side,
	                                       0.75f,
	                                       neurodraw::Tween::Easing::EaseInOut));
	engine_ptr->add_tween(neurodraw::Tween(&cluster,
	                                       neurodraw::Tween::Property::Rotation,
	                                       cluster.rotation() + 180.0f,
	                                       0.90f,
	                                       neurodraw::Tween::Easing::EaseInOut));
	engine_ptr->add_tween(neurodraw::Tween(&beacon,
	                                       neurodraw::Tween::Property::Alpha,
	                                       0.25f,
	                                       0.35f,
	                                       neurodraw::Tween::Easing::EaseOut));
	engine_ptr->add_tween(neurodraw::Tween(&beacon,
	                                       neurodraw::Tween::Property::Alpha,
	                                       0.95f,
	                                       0.85f,
	                                       neurodraw::Tween::Easing::EaseIn));
}

void callback_mouse(const neurodraw::MouseEvent& event) {
	float world_x = 0.0f;
	float world_y = 0.0f;
	engine_ptr->mouse_to_world(event.x, event.y, &world_x, &world_y);

	cursor.move(world_x, world_y);

	reset_strokes();
	hover_frame.set_alpha(0.0f);
	status.set_text("Hover a shape");

	neurodraw::Shape* hit = engine_ptr->hit(world_x, world_y);
	if(hit == &panel) {
		panel.set_stroke_color(neurodraw::Palette::aquamarine);
		panel.set_stroke_width(5.0f);
		place_hover_frame(&panel);
		hover_frame.set_alpha(1.0f);
		status.set_text("Panel hit");
	} else if(hit == &beacon) {
		beacon.set_stroke_color(neurodraw::Palette::aquamarine);
		beacon.set_stroke_width(5.0f);
		place_hover_frame(&beacon);
		hover_frame.set_alpha(1.0f);
		status.set_text("Beacon hit");
	} else if(hit == &pointer) {
		pointer.set_stroke_color(neurodraw::Palette::aquamarine);
		pointer.set_stroke_width(5.0f);
		place_hover_frame(&pointer);
		hover_frame.set_alpha(1.0f);
		status.set_text("Pointer hit");
	} else if(hit == &cluster) {
		place_hover_frame(&cluster);
		hover_frame.set_alpha(1.0f);
		status.set_text("Group bounds hit");
	}

	if(event.state == SDL_PRESSED && event.button == SDL_BUTTON_LEFT)
		queue_cluster_animation();
}

void callback_keyboard(const neurodraw::KeyboardEvent& event) {
	if(event.state != 1)
		return;

	switch(event.sym) {
		case neurodraw::EventKey::ESCAPE:
			engine_ptr->quit();
			break;
		case neurodraw::EventKey::SPACE:
			queue_cluster_animation();
			break;
	}
}

}  // namespace

int main(int argc, char** argv) {
	(void) argc;
	(void) argv;

	neurodraw::Engine engine("NeuroDraw Scene Showcase", 60.0f);
	engine_ptr = &engine;

	setup_scene();

	engine.on_keyboard(callback_keyboard);
	engine.on_mouse(callback_mouse);

	engine.add(&background_layer);
	engine.add(&scene_layer);
	engine.add(&overlay_layer);

	std::printf("ESC: quit | SPACE or left click: tween group | move mouse: hit test\n");

	while(engine.is_running()) {}

	return 0;
}
