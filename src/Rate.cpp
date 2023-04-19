#include "neurodraw/Rate.h"

namespace neurochrono {


Rate::Rate(float rate) {
	this->msecs_ = std::floor( (1.0f/rate)*1000 );
	this->timer_.tic();
}

Rate::~Rate(void) {}


void Rate::sleep(void) {

	auto difftime = this->msecs_ - this->timer_.toc();
	
	if(difftime < 0 || this->msecs_ == 0) {
		this->timer_.tic();
		return;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(difftime));
	this->timer_.tic();

}



}
