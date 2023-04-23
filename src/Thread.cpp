#include "neurodraw/Thread.h"

namespace neurodraw {

Thread::Thread(void) : run_(false), name_("engine") {}

Thread::~Thread(void) {}

void Thread::start(void) {

	if(this->is_running() == true) {
		this->stop();
		this->join();
	}

	printf("[%s] Thread is requested to start\n", this->name().c_str());
	this->thread_ = std::thread(&Thread::run, this);
	this->mutex_.lock();
	this->run_ = true;
	this->mutex_.unlock();
	
}

void Thread::stop(void) {
	printf("[%s] Thread is requested to stop\n", this->name().c_str());
	this->mutex_.lock();
	this->run_ = false;
	this->mutex_.unlock();
}

void Thread::teardown(void) {
	printf("[%s] Thread is requested to tearing down\n", this->name().c_str());
	if(this->is_running() == true) {
		this->stop();
	}
	this->join();
	printf("[%s] Thread has teared down\n", this->name().c_str());
}

bool Thread::is_running(void) {
	bool ret = false;
	this->mutex_.lock();
	ret = this->run_;
	this->mutex_.unlock();
	return ret;
}

void Thread::join(void) {
	printf("[%s] Thread is requested to join\n", this->name().c_str());
	if(this->thread_.joinable() == true)
		this->thread_.join();
}

void Thread::lock(void) {
	this->mutex_.lock();
}

void Thread::unlock(void) {
	this->mutex_.unlock();
}

std::string Thread::name(void) {
	std::string name;
	this->mutex_.lock();
	name = this->name_;
	this->mutex_.unlock();
	return name;
}




}
