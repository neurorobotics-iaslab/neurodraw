#include "neurodraw/Window.h"

namespace neurodraw {

Window::Window(const std::string& caption) : caption_(caption), width_(1024), 
											   height_(768), bpp_(24), 
											   x_(0), y_(0) {

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
	retcode = this->open_window(this->width_, this->height_, this->caption_);
	this->is_open_ = true;
	this->unlock();
	
	return retcode;
}

void Window::close(void) {
	this->lock();
	this->is_open_ = false;
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
	SDL_GL_SwapBuffers();			
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
	this->color_.at(0) = color.at(0);
	this->color_.at(1) = color.at(1);
	this->color_.at(2) = color.at(2);
	glClearColor(this->color_.at(1), this->color_.at(1), 
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
	SDL_Event evt_resize;
	KeyboardEvent evt_keyboard;
	MouseEvent evt_mouse;
	bool has_quit_event = false;
	bool has_videoexpose_event = false;
	bool has_videoresize_event = false;
	bool has_keyboard_event = false;
	bool has_mouse_event = false;

	// Consume all events
	while (SDL_PollEvent(&sevt)) {

		// Associate last event to callback
		switch (sevt.type) {
			case SDL_QUIT:
				has_quit_event = true;
				break;

			case SDL_VIDEOEXPOSE:
				has_videoexpose_event = true;
				break;

			case SDL_VIDEORESIZE:
				has_videoresize_event = true;
				evt_resize = sevt;
				break;

			case SDL_KEYUP:
			case SDL_KEYDOWN:
				has_keyboard_event = true;
				evt_keyboard = KeyboardEvent(sevt);
				break;

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEMOTION:
				has_mouse_event = true;
				evt_mouse = MouseEvent(sevt);
				break;
		}
	}

	if(has_quit_event == true & this->on_quit_func_ != nullptr)
		this->on_quit_func_();

	if(has_videoexpose_event == true & this->on_redraw_func_ != nullptr)
		this->on_redraw_func_();
	
	if(has_videoresize_event == true) {
		this->resize_window(evt_resize.resize.w, evt_resize.resize.h, 0);
		this->init_opengl_state();
	}
	
	if(has_keyboard_event == true & this->on_keyboard_func_ != nullptr)
		this->on_keyboard_func_(evt_keyboard);

	if(has_mouse_event == true & this->on_mouse_func_ != nullptr)
		this->on_mouse_func_(evt_mouse);


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
	//ratio = (float)wnd->width/(float)wnd->height;
	ratio = static_cast<float>(this->width_)/static_cast<float>(this->height_);
	iw = (ratio > 1.0f) ? ratio : 1.0f;
	ih = (ratio > 1.0f) ? 1.0f : 1.0f/ratio;
	glOrtho(-iw, iw, -ih, ih, -1, 1);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Init identity for modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Enable 2D texturing
	glEnable(GL_TEXTURE_2D);

	// Set clear color and clear blackground
	glClearColor(this->color_.at(0),this->color_.at(1),
				 this->color_.at(2), this->color_.at(3));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup transparency
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

	int fullscreen = 0;

	// Initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		fprintf(stderr,"SDL could not be initialized\n");
		SDL_Quit();
		return 0;
	}

	// Init parameters of the frame buffers
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 0 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	// Create resizable window using OpenGL
	if (!width || !height)
		fullscreen = 1;
	this->resize_window(width, height, fullscreen);
	
	// Set caption
	SDL_WM_SetCaption(caption.c_str(), NULL);

	if (this->init_opengl_state()) {
		return 0;
	}

	return 1;
}

int Window::resize_window(int width, int height, int fullscreen) {
	
	int flags = SDL_OPENGL | SDL_RESIZABLE | SDL_ANYFORMAT;
	const SDL_VideoInfo* info = NULL;
	SDL_Surface* surf;

	info = SDL_GetVideoInfo();
	if(info == NULL) {
		fprintf(stderr, "SDL_GetVideoInfo was kicked out.\n");
		return 1;
	}

	// Set window settings
	width  = (width > 0) ? width : info->current_w;
	height = (height > 0) ? height : info->current_h;  

	if (fullscreen)
		flags |= SDL_FULLSCREEN;

	// Create resizable window using OpenGL
	surf = SDL_SetVideoMode(width, height, this->bpp_, flags);
	if (!surf) {
		fprintf(stderr, "Window could not be loaded!\n");
		return 1;
	}

	this->window_ = surf;
	this->width_  = surf->w;
	this->height_ = surf->h;
	
	return 0;

}

}
