#include "neurodraw/Display.h"

namespace neurodraw {


Display::Display(const std::string& title, float rate) : rate_(rate) {
	this->lock();
	this->name_    = "display";
	this->window_  = new Window(title);;
	this->unlock();

	this->start();
}


Display::~Display(void) {

	printf("[%s] Destructor\n", this->name().c_str());
	this->teardown();

	this->window_->close();

	if(this->window_ != nullptr) 
		delete this->window_;
	
	printf("Closing display\n");
}

void Display::quit(void) {
	this->teardown();
}

void Display::run(void) {
	
	neurochrono::Rate r(this->rate_);
	neurochrono::timer_msecs timer;	
	
	printf("[%s] Engine is up\n", this->name().c_str());


	// Creating window
	this->window_->open();
	this->window_->make_current();
	this->window_->set_event_handler(func_event_handler);

	while(this->is_running() && this->window_->is_open()) {
		timer.tic();

		
		this->window_->clear();
		this->update();
		this->window_->update();

		this->window_->process_events();

		r.sleep();

		this->check_refresh_rate(timer.toc());
	}
	
	printf("[%s] Engine is down\n", this->name().c_str());
}




void Display::update(void) {

	this->lock();
	for(auto it=this->list_.begin(); it!=this->list_.end(); ++it)
		(*it)->render();
	this->unlock();
}

void Display::check_refresh_rate(int elapsed_msecs) {

	float threshold;

	threshold = (1 + this->rate_tollerance_/100.0f) * ( (1.0f/this->rate_)*1000 );

	if(elapsed_msecs > threshold ) {
		printf("[%s] Display is running slow (more than %2.1f%% of the set frequency rate)\n", 
				this->name().c_str(), this->rate_tollerance_);
	}
}

bool Display::draw(Shape* shape) {

	bool retcod;

	this->lock();
	auto p = this->list_.insert(shape);
	retcod = p.second;
	this->unlock();

	return retcod;
}

bool Display::clear(Shape* shape) {

	bool retcod;
	
	this->lock();
	size_t nelem = this->list_.erase(shape);
	retcod = nelem > 0 ? true: false;
	this->unlock();

	return retcod;

}

void Display::size(unsigned int* width, unsigned int* height) {
	this->window_->getsize(width, height);
}

}
