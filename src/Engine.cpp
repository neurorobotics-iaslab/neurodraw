#include "neurodraw/Engine.h"

#include <chrono>
#include <cstdio>

namespace neurodraw {


Engine::Engine(const std::string& wtitle, float fps) : fps_(fps) {
	this->name_ = "display";
	this->window_.reset(new Window(wtitle));
	this->set_fps_tolerance(NEURODRAW_FPS_TOLERANCE);

	this->start();
}


Engine::~Engine(void) {

	printf("[%s] Destructor\n", this->name().c_str());
	this->teardown();

	if(this->window_ != nullptr)
		this->window_->close();
}

void Engine::run(void) {

	neurochrono::Rate r(this->fps_);
	float current_fps;
	auto last_tick = std::chrono::steady_clock::now();

	printf("[%s] Engine is up\n", this->name().c_str());

	if(!this->window_->open()) {
		printf("[%s] Engine could not open the window\n", this->name().c_str());
		this->stop();
		return;
	}

	// Register default callback
	this->window_->on_quit(&Engine::callback_quit, this);
	this->window_->on_redraw(&Engine::callback_redraw, this);

	// Start main loop
	while(this->is_running()) {
		const auto now = std::chrono::steady_clock::now();
		const std::chrono::duration<float> dt = now - last_tick;
		last_tick = now;
		this->lock();
		this->animator_.update(dt.count());
		this->unlock();

		// Clear window
		this->window_->clear();

		// Draw all shapes in the engine
		this->update();

		// Update window
		this->window_->update();

		// Process window events
		this->window_->process_events();

		r.sleep();

		current_fps = this->real_fps();
		if(current_fps < this->fps_minimum()) {
			printf("[%s] Running late: %3.1f fps vs. %3.1f fps (nominal %3.1f fps)\n",
				   this->name().c_str(),
				   current_fps, this->fps_minimum(),
				   this->fps());
		}
	}

	printf("[%s] Engine is down\n", this->name().c_str());

	// Closing window
	this->window_->close();
}

void Engine::callback_quit(void) {
	this->stop();
}

void Engine::callback_redraw(void) {
	this->update();
}

void Engine::quit(void) {
	this->stop();
}

void Engine::on_keyboard(std::function<void(const KeyboardEvent& kevt)> func) {
	this->lock();
	this->window_->on_keyboard(func);
	this->unlock();
}

void Engine::on_mouse(std::function<void(const MouseEvent& mevt)> func) {
	this->lock();
	this->window_->on_mouse(func);
	this->unlock();
}

void Engine::set_fps_tolerance(float tolerance) {
	this->lock();
	this->fps_minimum_ = ((100.0f - tolerance) * this->fps_) / 100.0f;
	this->unlock();
	printf("[%s] Minimum fps set at: %3.1f fps (nominal %3.1f fps, tolerance %2.1f%%)\n",
			this->name().c_str(),
		    this->fps_minimum(),
			this->fps(),
			tolerance);
}

void Engine::winsize(unsigned int* width, unsigned int* height) {
	this->window_->size(width, height);
}

float Engine::fps(void) {
	float fps;
	this->lock();
	fps = this->fps_;
	this->unlock();
	return fps;
}

float Engine::fps_minimum(void) {
	float fps_minimum_;
	this->lock();
	fps_minimum_ = this->fps_minimum_;
	this->unlock();
	return fps_minimum_;
}

void Engine::update(void) {

	this->lock();
	for(auto it = this->list_.begin(); it != this->list_.end(); ++it)
		(*it)->draw();
	this->unlock();
}

float Engine::real_fps(void) {

	static neurochrono::timer_msecs timer;
	float real_fps;

	this->lock();
	float elapsed = timer.toc();
	timer.tic();
	this->unlock();

	if(elapsed <= 0.0f)
		return 0.0f;

	real_fps = 1000.0f / elapsed;
	return real_fps;
}

bool Engine::add(Shape* shape) {

	if(shape == nullptr)
		return false;

	this->lock();
	this->list_.push_back(shape);
	this->unlock();

	return true;
}

Shape* Engine::hit(float x, float y) {
	this->lock();
	for(auto it = this->list_.rbegin(); it != this->list_.rend(); ++it) {
		if(*it == nullptr)
			continue;

		Shape* hit_shape = (*it)->hit(x, y);
		if(hit_shape != nullptr) {
			this->unlock();
			return hit_shape;
		}
	}
	this->unlock();
	return nullptr;
}

void Engine::mouse_to_world(int pixel_x, int pixel_y, float* x, float* y) {
	unsigned int width = 0;
	unsigned int height = 0;
	this->winsize(&width, &height);
	if(width == 0 || height == 0)
		return;

	const float ratio = static_cast<float>(width) / static_cast<float>(height);
	const float world_half_width = (ratio > 1.0f) ? ratio : 1.0f;
	const float world_half_height = (ratio > 1.0f) ? 1.0f : 1.0f / ratio;

	if(x != nullptr)
		*x = ((2.0f * static_cast<float>(pixel_x)) / static_cast<float>(width) - 1.0f) * world_half_width;
	if(y != nullptr)
		*y = (1.0f - (2.0f * static_cast<float>(pixel_y)) / static_cast<float>(height)) * world_half_height;
}

bool Engine::erase(unsigned int pos) {

	std::size_t lsize;
	this->lock();
	lsize = this->list_.size();
	this->unlock();

	if(pos >= lsize)
		return false;

	this->lock();
	auto it = this->list_.begin();
	std::advance(it, pos);
	this->list_.erase(it);
	this->unlock();

	return true;

}

void Engine::clear(void) {
	this->lock();
	this->list_.clear();
	this->unlock();
}

void Engine::add_tween(const Tween& tween) {
	this->lock();
	this->animator_.add(tween);
	this->unlock();
}

void Engine::clear_tweens(void) {
	this->lock();
	this->animator_.clear();
	this->unlock();
}

std::size_t Engine::tween_count(void) {
	this->lock();
	const std::size_t count = this->animator_.size();
	this->unlock();
	return count;
}


}
