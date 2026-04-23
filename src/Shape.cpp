#include "neurodraw/Shape.h"

#include <algorithm>

namespace neurodraw {

Shape::Shape(void) {
	this->x_ = 0.0f;
	this->y_ = 0.0f;
	this->rot_ = 0.0f;
	this->primtype_ = GL_LINE_LOOP;
	this->outline_primtype_ = GL_LINE_LOOP;
	this->primary_is_fill_ = false;
	this->has_outline_ = false;
	this->stroke_width_ = 1.0f;
}

Shape::~Shape(void) {}

void Shape::add_point(float x, float y) {
	this->lock();
	this->points_.push_back(x);
	this->points_.push_back(y);
	this->colors_.insert(this->colors_.end(), 4, 1.0f);
	this->outline_colors_.insert(this->outline_colors_.end(), 4, 1.0f);
	this->unlock();
}

void Shape::add_index(unsigned int index) {
	this->lock();
	this->indices_.push_back(index);
	this->unlock();
}

void Shape::add_fill_index(unsigned int index) {
	this->lock();
	this->indices_.push_back(index);
	this->unlock();
}

void Shape::add_stroke_index(unsigned int index) {
	this->lock();
	this->outline_indices_.push_back(index);
	this->unlock();
}

void Shape::add_outline_index(unsigned int index) {
	this->add_stroke_index(index);
}

unsigned int Shape::num_points(void) {
	unsigned int npoints;
	this->lock();
	npoints = this->points_.size() / 2.0f;
	this->unlock();
	return npoints;
}

std::vector<float> Shape::points(void) {

	std::vector<float> points;
	this->lock();
	points = this->points_;
	this->unlock();

	return points;
}

std::vector<unsigned int> Shape::indices(void) {

	std::vector<unsigned int> indices;
	this->lock();
	indices = this->indices_;
	this->unlock();
	return indices;
}

std::vector<float> Shape::colors(void) {

	std::vector<float> colors;
	this->lock();
	colors = this->colors_;
	this->unlock();

	return colors;
}


void Shape::lock(void) {
	this->mutex_.lock();
}

void Shape::unlock(void) {
	this->mutex_.unlock();
}

void Shape::move(float x, float y) {
	this->lock();
	this->x_ = x;
	this->y_ = y;
	this->unlock();
}

void Shape::relmove(float dx, float dy) {
	this->lock();
	this->x_ += dx;
	this->y_ += dy;
	this->unlock();
}

void Shape::rotate(float deg) {
	this->lock();
	this->rot_ = deg;
	this->unlock();
}

void Shape::relrotate(float ddeg) {
	this->lock();
	this->rot_ += ddeg;
	this->unlock();
}

void Shape::rotate(float deg, float refx, float refy, bool hold) {

	float cx, cy, crot, radius, rdeg;
	this->lock();
	cx = this->x_;
	cy = this->y_;
	crot = std::atan2(cy - refy, cx - refx) * 180.0f / M_PI;
	radius = std::sqrt(std::pow((refx - cx), 2) + std::pow((refy - cy), 2));
	rdeg = deg * M_PI / 180.0f;
	this->x_ = radius * std::cos(rdeg) + refx;
	this->y_ = radius * std::sin(rdeg) + refy;
	if(hold == false)
		this->rot_ = this->rot_ + deg - crot;

	this->unlock();
}

void Shape::relrotate(float ddeg, float refx, float refy, bool hold) {

	float cx, cy, crot, radius, rdeg;
	this->lock();
	cx = this->x_;
	cy = this->y_;
	crot = std::atan2(cy - refy, cx - refx) * 180.0f / M_PI;
	radius = std::sqrt(std::pow((refx - cx), 2) + std::pow((refy - cy), 2));
	rdeg = (ddeg + crot) * M_PI / 180.0f;
	this->x_ = radius * std::cos(rdeg) + refx;
	this->y_ = radius * std::sin(rdeg) + refy;
	if(hold == false)
		this->rot_ += ddeg;

	this->unlock();
}

void Shape::set_mesh_color(std::vector<float>& colors, Color color) {
	for(std::size_t i = 0; i < colors.size(); i += 4) {
		colors.at(i) = color.at(0) / 255.0f;
		colors.at(i + 1) = color.at(1) / 255.0f;
		colors.at(i + 2) = color.at(2) / 255.0f;
	}
}

void Shape::set_mesh_color(std::vector<float>& colors, Color color, unsigned int index) {
	unsigned int slot = 4 * index;
	if(slot + 2 >= colors.size())
		return;

	colors.at(slot) = color.at(0) / 255.0f;
	colors.at(slot + 1) = color.at(1) / 255.0f;
	colors.at(slot + 2) = color.at(2) / 255.0f;
}

void Shape::set_mesh_alpha(std::vector<float>& colors, float alpha) {
	for(std::size_t i = 3; i < colors.size(); i += 4)
		colors.at(i) = alpha;
}

void Shape::set_mesh_alpha(std::vector<float>& colors, float alpha, unsigned int index) {
	unsigned int slot = 4 * index + 3;
	if(slot >= colors.size())
		return;

	colors.at(slot) = alpha;
}

void Shape::set_color(Color color) {

	this->lock();
	this->set_mesh_color(this->colors_, color);
	if(this->has_outline_)
		this->set_mesh_color(this->outline_colors_, color);
	this->unlock();
}

void Shape::set_color(Color color, unsigned int index) {
	this->lock();
	this->set_mesh_color(this->colors_, color, index);
	if(this->has_outline_)
		this->set_mesh_color(this->outline_colors_, color, index);
	this->unlock();
}

void Shape::set_fill_color(Color color) {
	this->lock();
	if(this->primary_is_fill_)
		this->set_mesh_color(this->colors_, color);
	this->unlock();
}

void Shape::set_fill_color(Color color, unsigned int index) {
	this->lock();
	if(this->primary_is_fill_)
		this->set_mesh_color(this->colors_, color, index);
	this->unlock();
}

void Shape::set_stroke_color(Color color) {
	this->lock();
	if(this->primary_is_fill_) {
		if(this->has_outline_)
			this->set_mesh_color(this->outline_colors_, color);
	} else {
		this->set_mesh_color(this->colors_, color);
	}
	this->unlock();
}

void Shape::set_stroke_color(Color color, unsigned int index) {
	this->lock();
	if(this->primary_is_fill_) {
		if(this->has_outline_)
			this->set_mesh_color(this->outline_colors_, color, index);
	} else {
		this->set_mesh_color(this->colors_, color, index);
	}
	this->unlock();
}

void Shape::set_alpha(float alpha) {
	this->lock();
	this->set_mesh_alpha(this->colors_, alpha);
	if(this->has_outline_)
		this->set_mesh_alpha(this->outline_colors_, alpha);
	this->unlock();
}

void Shape::set_alpha(float alpha, unsigned int index) {
	this->lock();
	this->set_mesh_alpha(this->colors_, alpha, index);
	if(this->has_outline_)
		this->set_mesh_alpha(this->outline_colors_, alpha, index);
	this->unlock();
}

void Shape::set_fill_alpha(float alpha) {
	this->lock();
	if(this->primary_is_fill_)
		this->set_mesh_alpha(this->colors_, alpha);
	this->unlock();
}

void Shape::set_fill_alpha(float alpha, unsigned int index) {
	this->lock();
	if(this->primary_is_fill_)
		this->set_mesh_alpha(this->colors_, alpha, index);
	this->unlock();
}

void Shape::set_stroke_alpha(float alpha) {
	this->lock();
	if(this->primary_is_fill_) {
		if(this->has_outline_)
			this->set_mesh_alpha(this->outline_colors_, alpha);
	} else {
		this->set_mesh_alpha(this->colors_, alpha);
	}
	this->unlock();
}

void Shape::set_stroke_alpha(float alpha, unsigned int index) {
	this->lock();
	if(this->primary_is_fill_) {
		if(this->has_outline_)
			this->set_mesh_alpha(this->outline_colors_, alpha, index);
	} else {
		this->set_mesh_alpha(this->colors_, alpha, index);
	}
	this->unlock();
}

void Shape::hide(void) {
	this->set_alpha(0.0f);
}

void Shape::show(void) {
	this->set_alpha(1.0f);
}

float Shape::x(void) {
	float x;
	this->lock();
	x = this->x_;
	this->unlock();
	return x;
}

float Shape::y(void) {
	float y;
	this->lock();
	y = this->y_;
	this->unlock();
	return y;
}

float Shape::rotation(void) {
	float rot;
	this->lock();
	rot = this->rot_;
	this->unlock();
	return rot;
}

float Shape::alpha(void) {
	float alpha = 1.0f;
	this->lock();
	if(this->colors_.size() >= 4)
		alpha = this->colors_.at(3);
	this->unlock();
	return alpha;
}

void Shape::set_primitive_type(GLenum type) {
	this->lock();
	this->primtype_ = type;
	this->primary_is_fill_ = false;
	this->unlock();
}

void Shape::set_fill_primitive_type(GLenum type) {
	this->lock();
	this->primtype_ = type;
	this->primary_is_fill_ = true;
	this->unlock();
}

void Shape::set_stroke_primitive_type(GLenum type) {
	this->lock();
	this->outline_primtype_ = type;
	this->has_outline_ = true;
	this->unlock();
}

void Shape::enable_stroke(GLenum type) {
	this->set_stroke_primitive_type(type);
}

bool Shape::uses_fill_mesh(void) const {
	return this->primary_is_fill_;
}

bool Shape::has_stroke_mesh(void) const {
	return this->primary_is_fill_ ? this->has_outline_ : true;
}

void Shape::current_transform(float* x, float* y, float* rot) {
	this->lock();
	if(x != nullptr)
		*x = this->x_;
	if(y != nullptr)
		*y = this->y_;
	if(rot != nullptr)
		*rot = this->rot_;
	this->unlock();
}

void Shape::set_stroke_width(float width) {
	this->lock();
	this->stroke_width_ = (width > 0.0f) ? width : 1.0f;
	this->unlock();
}

float Shape::stroke_width(void) {
	float width;
	this->lock();
	width = this->stroke_width_;
	this->unlock();
	return width;
}

void Shape::draw(void) {

	this->lock();

	glPushMatrix();

	// Translate to the reference point
	glTranslatef(this->x_, this->y_, 0.0f);
	glRotatef(this->rot_, 0.0f, 0.0f, 1.0f);

	glVertexPointer(2, GL_FLOAT, 0, this->points_.data());

	if(!this->indices_.empty()) {
		glColorPointer(4, GL_FLOAT, 0, this->colors_.data());
		glDrawElements(this->primtype_, this->indices_.size(),
		               GL_UNSIGNED_INT, this->indices_.data());
	}

	if(this->has_outline_ && !this->outline_indices_.empty()) {
		glLineWidth(this->stroke_width_);
		glColorPointer(4, GL_FLOAT, 0, this->outline_colors_.data());
		glDrawElements(this->outline_primtype_, this->outline_indices_.size(),
		               GL_UNSIGNED_INT, this->outline_indices_.data());
		glLineWidth(1.0f);
	}

	glPopMatrix();
	this->unlock();
}

void Shape::local_bounds(float* min_x, float* min_y,
                         float* max_x, float* max_y) {

	float out_min_x = 0.0f;
	float out_min_y = 0.0f;
	float out_max_x = 0.0f;
	float out_max_y = 0.0f;

	this->lock();
	if(this->points_.size() >= 2) {
		out_min_x = out_max_x = this->points_.at(0);
		out_min_y = out_max_y = this->points_.at(1);
		for(std::size_t i = 2; i + 1 < this->points_.size(); i += 2) {
			out_min_x = std::min(out_min_x, this->points_.at(i));
			out_max_x = std::max(out_max_x, this->points_.at(i));
			out_min_y = std::min(out_min_y, this->points_.at(i + 1));
			out_max_y = std::max(out_max_y, this->points_.at(i + 1));
		}
	}
	this->unlock();

	if(min_x != nullptr)
		*min_x = out_min_x;
	if(min_y != nullptr)
		*min_y = out_min_y;
	if(max_x != nullptr)
		*max_x = out_max_x;
	if(max_y != nullptr)
		*max_y = out_max_y;
}

void Shape::bounds(float* min_x, float* min_y, float* max_x, float* max_y) {

	float local_min_x, local_min_y, local_max_x, local_max_y;
	float tx, ty, rot_deg;
	this->local_bounds(&local_min_x, &local_min_y, &local_max_x, &local_max_y);
	this->current_transform(&tx, &ty, &rot_deg);

	const float rot = rot_deg * static_cast<float>(M_PI) / 180.0f;
	const float cs = std::cos(rot);
	const float sn = std::sin(rot);

	const float corners[4][2] = {
		{local_min_x, local_min_y},
		{local_min_x, local_max_y},
		{local_max_x, local_min_y},
		{local_max_x, local_max_y}
	};

	float out_min_x = tx + corners[0][0] * cs - corners[0][1] * sn;
	float out_max_x = out_min_x;
	float out_min_y = ty + corners[0][0] * sn + corners[0][1] * cs;
	float out_max_y = out_min_y;

	for(int i = 1; i < 4; ++i) {
		const float px = tx + corners[i][0] * cs - corners[i][1] * sn;
		const float py = ty + corners[i][0] * sn + corners[i][1] * cs;
		out_min_x = std::min(out_min_x, px);
		out_max_x = std::max(out_max_x, px);
		out_min_y = std::min(out_min_y, py);
		out_max_y = std::max(out_max_y, py);
	}

	if(min_x != nullptr)
		*min_x = out_min_x;
	if(min_y != nullptr)
		*min_y = out_min_y;
	if(max_x != nullptr)
		*max_x = out_max_x;
	if(max_y != nullptr)
		*max_y = out_max_y;
}

bool Shape::contains(float x, float y) {

	float tx, ty, rot_deg;
	float local_min_x, local_min_y, local_max_x, local_max_y;
	this->current_transform(&tx, &ty, &rot_deg);
	this->local_bounds(&local_min_x, &local_min_y, &local_max_x, &local_max_y);

	const float rot = -rot_deg * static_cast<float>(M_PI) / 180.0f;
	const float dx = x - tx;
	const float dy = y - ty;
	const float local_x = dx * std::cos(rot) - dy * std::sin(rot);
	const float local_y = dx * std::sin(rot) + dy * std::cos(rot);

	return (local_x >= local_min_x && local_x <= local_max_x &&
	        local_y >= local_min_y && local_y <= local_max_y);
}

Shape* Shape::hit(float x, float y) {
	return this->contains(x, y) ? this : nullptr;
}


}
