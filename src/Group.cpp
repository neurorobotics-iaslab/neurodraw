#include "neurodraw/Group.h"

#include <algorithm>
#include <cmath>

namespace neurodraw {

Group::Group(void) {}

Group::~Group(void) {}

bool Group::add(Shape* shape) {
	if(shape == nullptr)
		return false;

	this->lock_children();
	this->children_.push_back(shape);
	this->unlock_children();
	return true;
}

bool Group::erase(unsigned int index) {
	this->lock_children();
	if(index >= this->children_.size()) {
		this->unlock_children();
		return false;
	}

	this->children_.erase(this->children_.begin() + index);
	this->unlock_children();
	return true;
}

void Group::clear(void) {
	this->lock_children();
	this->children_.clear();
	this->unlock_children();
}

std::size_t Group::size(void) {
	this->lock_children();
	const std::size_t size = this->children_.size();
	this->unlock_children();
	return size;
}

Shape* Group::at(unsigned int index) {
	this->lock_children();
	Shape* shape = (index < this->children_.size()) ? this->children_.at(index) : nullptr;
	this->unlock_children();
	return shape;
}

void Group::draw(void) {
	float tx, ty, rot;
	this->current_transform(&tx, &ty, &rot);

	glPushMatrix();
	glTranslatef(tx, ty, 0.0f);
	glRotatef(rot, 0.0f, 0.0f, 1.0f);

	this->lock_children();
	for(auto child : this->children_) {
		if(child != nullptr)
			child->draw();
	}
	this->unlock_children();

	glPopMatrix();
}

void Group::local_bounds(float* min_x, float* min_y,
                         float* max_x, float* max_y) {
	float out_min_x = 0.0f;
	float out_min_y = 0.0f;
	float out_max_x = 0.0f;
	float out_max_y = 0.0f;
	bool has_child = false;

	this->lock_children();
	for(auto child : this->children_) {
		if(child == nullptr)
			continue;

		float child_min_x, child_min_y, child_max_x, child_max_y;
		child->bounds(&child_min_x, &child_min_y, &child_max_x, &child_max_y);

		if(!has_child) {
			out_min_x = child_min_x;
			out_min_y = child_min_y;
			out_max_x = child_max_x;
			out_max_y = child_max_y;
			has_child = true;
			continue;
		}

		out_min_x = std::min(out_min_x, child_min_x);
		out_min_y = std::min(out_min_y, child_min_y);
		out_max_x = std::max(out_max_x, child_max_x);
		out_max_y = std::max(out_max_y, child_max_y);
	}
	this->unlock_children();

	if(min_x != nullptr)
		*min_x = out_min_x;
	if(min_y != nullptr)
		*min_y = out_min_y;
	if(max_x != nullptr)
		*max_x = out_max_x;
	if(max_y != nullptr)
		*max_y = out_max_y;
}

bool Group::contains(float x, float y) {
	return this->hit(x, y) != nullptr;
}

Shape* Group::hit(float x, float y) {
	float tx, ty, rot_deg;
	this->current_transform(&tx, &ty, &rot_deg);

	const float rot = -rot_deg * static_cast<float>(M_PI) / 180.0f;
	const float dx = x - tx;
	const float dy = y - ty;
	const float local_x = dx * std::cos(rot) - dy * std::sin(rot);
	const float local_y = dx * std::sin(rot) + dy * std::cos(rot);

	this->lock_children();
	for(auto it = this->children_.rbegin(); it != this->children_.rend(); ++it) {
		if(*it == nullptr)
			continue;

		Shape* hit_shape = (*it)->hit(local_x, local_y);
		if(hit_shape != nullptr) {
			this->unlock_children();
			return hit_shape;
		}
	}
	this->unlock_children();

	return Shape::contains(x, y) ? this : nullptr;
}

void Group::lock_children(void) {
	this->children_mutex_.lock();
}

void Group::unlock_children(void) {
	this->children_mutex_.unlock();
}

}
