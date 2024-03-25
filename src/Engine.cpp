#include "neurodraw/Engine.h"

namespace neurodraw {


Engine::Engine(const std::string& wtitle, float fps) : fps_(fps) {
	this->name_ = "display";
	this->window_ = new Window(wtitle);
	this->set_fps_tolerance(NEURODRAW_FPS_TOLERANCE);

	this->start();
}


Engine::~Engine(void) {

	printf("[%s] Destructor\n", this->name().c_str());
	this->teardown();

	while(this->window_->is_open());

	// Destroy window
	if(this->window_ != nullptr) 
		delete this->window_;
}

void Engine::run(void) {
	
	neurochrono::Rate r(this->fps_);
	float current_fps;
	
	printf("[%s] Engine is up\n", this->name().c_str());

	// Open window
	this->window_->open();

	// Register default callback
	this->window_->on_quit(&Engine::callback_quit, this);
	this->window_->on_redraw(&Engine::callback_redraw, this);

	// Start main loop
	while(this->is_running()) {

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
	this->fps_minimum_ = ( (100.0f - tolerance) * this->fps_ ) / 100.0f;
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
	for(auto it=this->list_.begin(); it!=this->list_.end(); ++it)
		(*it)->draw();
	this->unlock();
}

float Engine::real_fps(void) {

	static neurochrono::timer_msecs timer;
	float real_fps;

	this->lock();
	real_fps = 1000.0f / timer.toc();
	timer.tic();
	this->unlock();

	return real_fps;
}

bool Engine::add(Shape* shape) {

	bool retcod = true;

	this->lock();
	this->list_.push_back(shape);
	this->unlock();

	return retcod;
}

bool Engine::erase(unsigned int pos) {

	int lsize;
	this->lock();
	lsize = this->list_.size();
	this->unlock();

	if(pos > lsize - 1 || pos < 0)
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


}
