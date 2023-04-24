#include "neurodraw/Shape.h"


namespace neurodraw {

Shape::Shape(void) {
	this->x_        = 0.0f;
	this->y_        = 0.0f;
	this->rot_      = 0.0f;
	this->primtype_ = GL_LINE_LOOP;
}

Shape::~Shape(void) {}

void Shape::add_point(float x, float y) {
	this->lock();
	this->points_.push_back(x);
	this->points_.push_back(y);
	this->colors_.insert(this->colors_.end(), 4, 1.0f);
	this->unlock();
}

void Shape::add_index(unsigned int index) {
	this->lock();
	this->indices_.push_back(index);
	this->unlock();
}

unsigned int Shape::num_points(void) {
	unsigned int npoints;
	this->lock();
	npoints = this->points_.size()/2.0f;
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
	radius = std::sqrt( std::pow( (refx - cx) , 2 ) + std::pow( (refy - cy) , 2) );
	rdeg = deg * M_PI / 180.0f ;
	this->x_ = radius * std::cos( rdeg  ) + refx;
	this->y_ = radius * std::sin( rdeg  ) + refy;
	if(hold == false)
		this->rot_ = this->rot_ + deg - crot;

	this->unlock();
}

void Shape::relrotate(float ddeg, float refx, float refy, bool hold) {

	float cx, cy, crot, radius, rdeg;
	this->lock();
	cx   = this->x_;
	cy   = this->y_;
	crot = std::atan2(cy - refy, cx - refx) * 180.0f / M_PI;
	radius = std::sqrt( std::pow( (refx - cx) , 2 ) + std::pow( (refy - cy) , 2) );
	rdeg = (ddeg + crot) * M_PI / 180.0f ;
	this->x_ = radius * std::cos( rdeg  ) + refx;
	this->y_ = radius * std::sin( rdeg  ) + refy;
	if(hold == false)
		this->rot_ += this->rot_ + ddeg - crot;

	this->unlock();
}

void Shape::set_color(Color color) {

	this->lock();

	for(auto i = 0; i<this->colors_.size(); i+=4) {
		this->colors_.at(i)   = color.at(0)/255.0f;
		this->colors_.at(i+1) = color.at(1)/255.0f;
		this->colors_.at(i+2) = color.at(2)/255.0f;
	}

	this->unlock();
}

void Shape::set_color(Color color, unsigned int index) {

	unsigned int slot = 4 * index;

	this->lock();
	this->colors_.at(slot)   = color.at(0)/255.0f;
	this->colors_.at(slot+1) = color.at(1)/255.0f;
	this->colors_.at(slot+2) = color.at(2)/255.0f;
	this->unlock();
}

void Shape::set_alpha(float alpha) {
	this->lock();
	for(auto i = 3; i<this->colors_.size(); i+=4) {
		this->colors_.at(i)   = alpha;
	}
	this->unlock();
}

void Shape::set_alpha(float alpha, unsigned int index) {
	this->lock();
	this->colors_.at(4 * index + 3) = alpha;
	this->unlock();
}


void Shape::hide(void) {
	this->set_alpha(0.0f);
}

void Shape::show(void) {
	this->set_alpha(1.0f);
}

void Shape::set_primitive_type(GLenum type) {
	this->lock();
	this->primtype_ = type;
	this->unlock();
}

void Shape::draw(void) {

	this->lock();

	glPushMatrix();

	// Translate to the reference point
	glTranslatef(this->x_, this->y_, 0.0f);
	glRotatef(this->rot_, 0.0f, 0.0f, 1.0f); 

	glVertexPointer(2, GL_FLOAT, 0, this->points_.data());
	glColorPointer(4, GL_FLOAT, 0, this->colors_.data());

	// Draw shapes
	glDrawElements(this->primtype_, this->indices_.size(), 
	               GL_UNSIGNED_INT, this->indices_.data());

	// Reset drawing  position
	glPopMatrix(); 
	this->unlock();

}


}
