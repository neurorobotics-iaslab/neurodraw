#include "neurodraw/Shape.h"

namespace neurodraw {

Shape::Shape(void) {

	this->lock();	
	this->color_     = Palette::white;
	this->is_filled_ = false;
	this->shape_     = nullptr;
	this->x_ 		 = 0.0f;
	this->y_    	 = 0.0f;
	this->z_  	     = 0.0f;
	this->unlock();
}

Shape::~Shape(void) {
	this->lock();
	if(this->shape_ != nullptr)
		dtk_destroy_shape(this->shape_);
	this->unlock();
	
}

void Shape::lock(void) {
	this->mutex_.lock();
}

void Shape::unlock(void) {
	this->mutex_.unlock();
}

void Shape::move(float x, float y) {
	this->lock();
	if(this->shape_ != nullptr) {
		dtk_move_shape(this->shape_, x, y);
		this->x_ = x;
		this->y_ = y;
	}
	this->unlock();
}	

void Shape::rmove(float dx, float dy) {
	this->lock();
	if(this->shape_ != nullptr) {
		dtk_relmove_shape(this->shape_, dx, dy);
		this->x_ = this->x_ + dx;
		this->y_ = this->y_ + dy;
	}
	this->unlock();
}

void Shape::rotate(float deg) {
	this->lock();
	if(this->shape_ != nullptr) {
		dtk_rotate_shape(this->shape_, deg);
		this->z_ = deg;
	}
	this->unlock();
}

void Shape::rrotate(float ddeg) {
	this->lock();
	if(this->shape_ != nullptr) {
		dtk_relrotate_shape(this->shape_, ddeg);
		this->z_ = this->z_ + ddeg;
	}
	this->unlock();
}

void Shape::hide(void) {
	this->set_alpha(0.0f);
}

void Shape::show(void) {
	this->set_alpha(1.0f);
}

void Shape::fill(void) {
	this->lock();
	this->is_filled_ = true;
	this->unlock();
}

void Shape::unfill(void) {
	this->lock();
	this->is_filled_ = false;
	this->unlock();
}

void Shape::set_alpha(float alpha) {
	std::array<float, 4> color; 

	this->lock();
	if(this->shape_ != nullptr) {
		color = this->dtk_color_array(this->color_, alpha);
		dtk_setcolor_shape(this->shape_, color.data(), DTK_IGNRGB);
	}
	this->unlock();
}

void Shape::set_color(Color color, float alpha) {

	this->lock();
	if(this->shape_ != nullptr) {
		this->color_ = color;
		dtk_setcolor_shape(this->shape_, this->dtk_color_array(color, alpha).data(), 0);
	}
	this->unlock();
}

Color Shape::get_color(void) {
	std::array<float, 3> color;
	
	this->lock();
	for(auto i = 0; i<3; i++)
		color.at(i) = this->color_.at(i);
	this->unlock();

	return color;

}

float Shape::get_alpha(void) {
	float alpha;
	
	this->lock();
	alpha = this->color_.at(3);
	this->unlock();

	return alpha;

}

void Shape::render(void) {
	this->lock();
	if(this->shape_ != nullptr)
		dtk_draw_shape(this->shape_);
	this->unlock();
}

std::array<float, 4> Shape::dtk_color_array(Color color, float alpha) {

	std::array<float, 4> array;
	array.at(0) = static_cast<float>(color.at(0))/255.0f;
	array.at(1) = static_cast<float>(color.at(1))/255.0f;
	array.at(2) = static_cast<float>(color.at(2))/255.0f;
	array.at(3) = alpha;

	return array;
}

}

