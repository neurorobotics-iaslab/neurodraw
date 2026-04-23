#include "neurodraw/Window.h"

namespace neurodraw {

Window::Window(const std::string& caption) : caption_(caption), width_(1024),
											   height_(768), bpp_(24),
											   x_(0), y_(0), window_(nullptr),
											   context_(nullptr) {

	this->color_ = {0.0f, 0.0f, 0.0f, 1.0f};
	this->is_open_ = false;

	this->on_quit_func_     = nullptr;
	this->on_redraw_func_   = nullptr;
	this->on_keyboard_func_ = nullptr;
	this->on_mouse_func_    = nullptr;
}

Window::~Window(void) {
	this->close();
}

bool Window::open(void) {

	bool retcode;

	this->lock();
	retcode = (this->open_window(this->width_, this->height_, this->caption_) == 0);
	this->is_open_ = retcode;
	this->unlock();

	return retcode;
}

void Window::close(void) {
	this->lock();

	this->is_open_ = false;

	if(this->context_ != nullptr) {
		SDL_GL_DeleteContext(this->context_);
		this->context_ = nullptr;
	}

	if(this->window_ != nullptr) {
		SDL_DestroyWindow(this->window_);
		this->window_ = nullptr;
	}

	SDL_Quit();
	this->unlock();
}

void Window::clear(void) {
	this->lock();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->unlock();
}

void Window::update(void) {
	this->lock();
	if(this->window_ != nullptr)
		SDL_GL_SwapWindow(this->window_);
	this->unlock();
}

bool Window::is_open(void) {
	bool retcod;
	this->lock();
	retcod = this->is_open_;
	this->unlock();
	return retcod;
}

void Window::set_color(Color color) {

	this->lock();
	this->color_.at(0) = color.at(0) / 255.0f;
	this->color_.at(1) = color.at(1) / 255.0f;
	this->color_.at(2) = color.at(2) / 255.0f;
	glClearColor(this->color_.at(0), this->color_.at(1),
				 this->color_.at(2), this->color_.at(3));
	this->unlock();
}

void Window::size(unsigned int* width, unsigned int* height) {
	this->lock();
	*width  = this->width_;
	*height = this->height_;
	this->unlock();
}


void Window::lock(void) {
	this->mutex_.lock();
}

void Window::unlock(void) {
	this->mutex_.unlock();
}

void Window::on_quit(std::function<void(void)> func) {
	this->lock();
	this->on_quit_func_ = func;
	this->unlock();
}

void Window::on_redraw(std::function<void(void)> func) {
	this->lock();
	this->on_redraw_func_ = func;
	this->unlock();
}

void Window::on_keyboard(std::function<void(const KeyboardEvent& kevt)> func) {
	this->lock();
	this->on_keyboard_func_ = func;
	this->unlock();
}


void Window::on_mouse(std::function<void(const MouseEvent& mevt)> func) {
	this->lock();
	this->on_mouse_func_ = func;
	this->unlock();
}

void Window::process_events(void) {

	SDL_Event sevt;

	// Consume all events
	while (SDL_PollEvent(&sevt)) {

		switch (sevt.type) {
			case SDL_QUIT:
				if(this->on_quit_func_ != nullptr)
					this->on_quit_func_();
				break;

			case SDL_WINDOWEVENT:
				switch(sevt.window.event) {
					case SDL_WINDOWEVENT_EXPOSED:
						if(this->on_redraw_func_ != nullptr)
							this->on_redraw_func_();
						break;

					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						this->resize_window(sevt.window.data1, sevt.window.data2, 0);
						this->init_opengl_state();
						if(this->on_redraw_func_ != nullptr)
							this->on_redraw_func_();
						break;
				}
				break;

			case SDL_KEYUP:
			case SDL_KEYDOWN:
				if(this->on_keyboard_func_ != nullptr)
					this->on_keyboard_func_(KeyboardEvent(sevt));
				break;

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEMOTION:
				if(this->on_mouse_func_ != nullptr)
					this->on_mouse_func_(MouseEvent(sevt));
				break;
		}
	}
}


/*************************************************************************
 *                                                                       *
 *             Private functions (not thread-safe)                       *
 *                                                                       *
 *************************************************************************/
int Window::init_opengl_state(void) {

	float ratio, iw, ih;
	GLenum err;

	// Setup 2D projection
	glViewport(0, 0, this->width_, this->height_);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ratio = static_cast<float>(this->width_) / static_cast<float>(this->height_);
	iw = (ratio > 1.0f) ? ratio : 1.0f;
	ih = (ratio > 1.0f) ? 1.0f : 1.0f / ratio;
	glOrtho(-iw, iw, -ih, ih, -1, 1);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Init identity for modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Enable 2D texturing
	glEnable(GL_TEXTURE_2D);

	// Set clear color and clear background
	glClearColor(this->color_.at(0), this->color_.at(1),
				 this->color_.at(2), this->color_.at(3));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);

	// Check for errors
	err = glGetError();
	if(err != GL_NO_ERROR) {
		fprintf(stderr, "OpenGL could not be initialized! (err=0x%04x)\n", err);
		return 1;
	}

	return 0;
}

int Window::open_window(int width, int height, const std::string& caption) {

	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "SDL could not be initialized: %s\n", SDL_GetError());
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	this->window_ = SDL_CreateWindow(caption.c_str(),
									 SDL_WINDOWPOS_CENTERED,
									 SDL_WINDOWPOS_CENTERED,
									 width,
									 height,
									 SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if(this->window_ == nullptr) {
		fprintf(stderr, "Window could not be created: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	this->context_ = SDL_GL_CreateContext(this->window_);
	if(this->context_ == nullptr) {
		fprintf(stderr, "OpenGL context could not be created: %s\n", SDL_GetError());
		SDL_DestroyWindow(this->window_);
		this->window_ = nullptr;
		SDL_Quit();
		return 1;
	}

	SDL_GL_SetSwapInterval(1);

	this->resize_window(width, height, 0);

	if(this->init_opengl_state()) {
		return 1;
	}

	return 0;
}

int Window::resize_window(int width, int height, int fullscreen) {
	(void) fullscreen;

	if(this->window_ == nullptr)
		return 1;

	if(width <= 0 || height <= 0) {
		int drawable_width = 0;
		int drawable_height = 0;
		SDL_GL_GetDrawableSize(this->window_, &drawable_width, &drawable_height);
		width = drawable_width;
		height = drawable_height;
	}

	this->width_ = static_cast<unsigned int>(width);
	this->height_ = static_cast<unsigned int>(height);

	return 0;
}

}
