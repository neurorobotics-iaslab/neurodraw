#include "neurodraw/Tween.h"

#include <algorithm>
#include <cmath>

namespace neurodraw {

Tween::Tween(Shape* target, Property property, float to, float duration, Easing easing) :
	target_(target),
	property_(property),
	from_(0.0f),
	to_(to),
	duration_(duration > 0.0f ? duration : 0.0001f),
	elapsed_(0.0f),
	easing_(easing),
	capture_current_(true),
	finished_(false) {}

void Tween::bind(Shape* target) {
	this->target_ = target;
}

Shape* Tween::target(void) const {
	return this->target_;
}

void Tween::set_property(Property property) {
	this->property_ = property;
}

Tween::Property Tween::property(void) const {
	return this->property_;
}

void Tween::set_from(float from) {
	this->from_ = from;
	this->capture_current_ = false;
}

void Tween::use_current(void) {
	this->capture_current_ = true;
}

void Tween::set_to(float to) {
	this->to_ = to;
}

float Tween::to(void) const {
	return this->to_;
}

void Tween::set_duration(float duration) {
	this->duration_ = (duration > 0.0f) ? duration : 0.0001f;
}

float Tween::duration(void) const {
	return this->duration_;
}

void Tween::set_easing(Easing easing) {
	this->easing_ = easing;
}

Tween::Easing Tween::easing(void) const {
	return this->easing_;
}

void Tween::reset(void) {
	this->elapsed_ = 0.0f;
	this->finished_ = false;
}

bool Tween::advance(float dt) {
	if(this->finished_ || this->target_ == nullptr)
		return true;

	if(this->capture_current_) {
		this->from_ = this->current_value();
		this->capture_current_ = false;
	}

	this->elapsed_ += std::max(dt, 0.0f);
	const float t = std::min(this->elapsed_ / this->duration_, 1.0f);
	const float value = this->from_ + (this->to_ - this->from_) * this->eased(t);
	this->apply(value);

	if(t >= 1.0f)
		this->finished_ = true;

	return this->finished_;
}

bool Tween::finished(void) const {
	return this->finished_;
}

float Tween::current_value(void) const {
	if(this->target_ == nullptr)
		return 0.0f;

	switch(this->property_) {
		case Property::X:
			return this->target_->x();
		case Property::Y:
			return this->target_->y();
		case Property::Rotation:
			return this->target_->rotation();
		case Property::Alpha:
			return this->target_->alpha();
	}

	return 0.0f;
}

void Tween::apply(float value) {
	if(this->target_ == nullptr)
		return;

	switch(this->property_) {
		case Property::X:
			this->target_->move(value, this->target_->y());
			break;
		case Property::Y:
			this->target_->move(this->target_->x(), value);
			break;
		case Property::Rotation:
			this->target_->rotate(value);
			break;
		case Property::Alpha:
			this->target_->set_alpha(value);
			break;
	}
}

float Tween::eased(float t) const {
	switch(this->easing_) {
		case Easing::Linear:
			return t;
		case Easing::EaseIn:
			return t * t;
		case Easing::EaseOut:
			return 1.0f - (1.0f - t) * (1.0f - t);
		case Easing::EaseInOut:
			if(t < 0.5f)
				return 2.0f * t * t;
			return 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
	}

	return t;
}

Animator::Animator(void) {}

void Animator::add(const Tween& tween) {
	this->tweens_.push_back(tween);
}

void Animator::clear(void) {
	this->tweens_.clear();
}

std::size_t Animator::size(void) const {
	return this->tweens_.size();
}

void Animator::update(float dt) {
	for(auto it = this->tweens_.begin(); it != this->tweens_.end();) {
		if(it->advance(dt))
			it = this->tweens_.erase(it);
		else
			++it;
	}
}

}
