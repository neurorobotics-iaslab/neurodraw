#include "neurodraw/Engine.h"

namespace neurodraw {

Engine::Engine(void) {
	this->mutex_.lock();
	this->run_  = false;
	this->name_ = "engine"; 
	this->mutex_.unlock();
}

Engine::~Engine(void) {
	printf("[%s] Destructor\n", this->name().c_str());
}

void Engine::start(void) {

	if(this->is_running() == true) {
		this->stop();
		this->join();
	}

	printf("[%s] Engine is requested to start\n", this->name().c_str());
	this->thread_ = std::thread(&Engine::run, this);
	this->mutex_.lock();
	this->run_ = true;
	this->mutex_.unlock();
	
}

void Engine::stop(void) {
	printf("[%s] Engine is requested to stop\n", this->name().c_str());
	this->mutex_.lock();
	this->run_ = false;
	this->mutex_.unlock();
}

void Engine::teardown(void) {
	printf("[%s] Engine is requested to tearing down\n", this->name().c_str());
	if(this->is_running() == true) {
		this->stop();
	}
	this->join();
	printf("[%s] Engine has teared down\n", this->name().c_str());
}

bool Engine::is_running(void) {
	bool ret = false;
	this->mutex_.lock();
	ret = this->run_;
	this->mutex_.unlock();
	return ret;
}

void Engine::join(void) {
	printf("[%s] Engine is requested to join\n", this->name().c_str());
	if(this->thread_.joinable() == true)
		this->thread_.join();
}

void Engine::lock(void) {
	this->mutex_.lock();
}

void Engine::unlock(void) {
	this->mutex_.unlock();
}

std::string Engine::name(void) {
	std::string name;
	this->mutex_.lock();
	name = this->name_;
	this->mutex_.unlock();
	return name;
}




}
