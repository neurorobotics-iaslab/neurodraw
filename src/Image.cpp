#include "neurodraw/Image.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

namespace neurodraw {

namespace {

struct ImageGuard {
	ImageGuard() {
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
	}

	~ImageGuard() = default;
};

static ImageGuard g_image_guard;

struct TextureData {
	TextureData()
		: texture(0),
		  width(0),
		  height(0),
		  pixel_data(),
		  dirty(false) {}

	unsigned int texture;
	int width;
	int height;
	std::vector<unsigned char> pixel_data;
	bool dirty;
};

std::string read_ppm_token(std::istream& input) {
	std::string token;
	char c = '\0';

	while(input.get(c)) {
		if(c == '#') {
			std::string comment;
			std::getline(input, comment);
			continue;
		}

		if(!std::isspace(static_cast<unsigned char>(c))) {
			token.push_back(c);
			break;
		}
	}

	while(input.get(c)) {
		if(std::isspace(static_cast<unsigned char>(c)))
			break;
		token.push_back(c);
	}

	return token;
}

SDL_Surface* load_ppm_surface(const std::string& path) {
	std::ifstream file(path.c_str(), std::ios::binary);
	if(!file.is_open())
		return nullptr;

	const std::string magic = read_ppm_token(file);
	if(magic != "P3")
		return nullptr;

	const int width = std::stoi(read_ppm_token(file));
	const int height = std::stoi(read_ppm_token(file));
	const int max_value = std::stoi(read_ppm_token(file));
	if(width <= 0 || height <= 0 || max_value <= 0)
		return nullptr;

	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
	if(surface == nullptr)
		return nullptr;

	Uint32* pixels = static_cast<Uint32*>(surface->pixels);
	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			const int r = std::stoi(read_ppm_token(file));
			const int g = std::stoi(read_ppm_token(file));
			const int b = std::stoi(read_ppm_token(file));

			const Uint8 rr = static_cast<Uint8>((255 * r) / max_value);
			const Uint8 gg = static_cast<Uint8>((255 * g) / max_value);
			const Uint8 bb = static_cast<Uint8>((255 * b) / max_value);
			pixels[y * width + x] = SDL_MapRGBA(surface->format, rr, gg, bb, 255);
		}
	}

	return surface;
}

SDL_Surface* load_surface(const std::string& path) {
	const std::size_t dot = path.find_last_of('.');
	const std::string extension = (dot == std::string::npos) ? "" : path.substr(dot + 1);

	if(extension == "png" || extension == "PNG" ||
	   extension == "jpg" || extension == "JPG" ||
	   extension == "jpeg" || extension == "JPEG" ||
	   extension == "gif" || extension == "GIF") {
		return IMG_Load(path.c_str());
	}

	if(extension == "bmp" || extension == "BMP")
		return SDL_LoadBMP(path.c_str());

	if(extension == "ppm" || extension == "PPM")
		return load_ppm_surface(path);

	return nullptr;
}

SDL_Surface* create_placeholder_surface() {
	const int size = 64;
	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, size, size, 32, SDL_PIXELFORMAT_RGBA32);
	if(surface == nullptr)
		return nullptr;

	const int tile = 8;
	Uint32* pixels = static_cast<Uint32*>(surface->pixels);
	for(int y = 0; y < size; ++y) {
		for(int x = 0; x < size; ++x) {
			const bool even = ((x / tile) + (y / tile)) % 2 == 0;
			const Uint8 r = even ? 235 : 60;
			const Uint8 g = even ? 60 : 20;
			const Uint8 b = even ? 235 : 20;
			pixels[y * size + x] = SDL_MapRGBA(surface->format, r, g, b, 255);
		}
	}

	return surface;
}

std::shared_ptr<TextureData> get_placeholder_texture_data() {
	static std::weak_ptr<TextureData> weak_placeholder;
	std::shared_ptr<TextureData> cached = weak_placeholder.lock();
	if(cached != nullptr)
		return cached;

	SDL_Surface* loaded = create_placeholder_surface();
	if(loaded == nullptr)
		return nullptr;

	SDL_Surface* rgba = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(loaded);
	if(rgba == nullptr)
		return nullptr;

	std::shared_ptr<TextureData> texture(new TextureData());
	texture->width = rgba->w;
	texture->height = rgba->h;
	texture->pixel_data.assign(static_cast<unsigned char*>(rgba->pixels),
	                           static_cast<unsigned char*>(rgba->pixels) + (rgba->h * rgba->pitch));
	texture->dirty = true;
	SDL_FreeSurface(rgba);

	weak_placeholder = texture;
	return texture;
}

std::shared_ptr<TextureData> load_texture_data(const std::string& path) {
	static std::map<std::string, std::weak_ptr<TextureData> > cache;

	const auto it = cache.find(path);
	if(it != cache.end()) {
		std::shared_ptr<TextureData> cached = it->second.lock();
		if(cached != nullptr)
			return cached;
	}

	SDL_Surface* loaded = load_surface(path);
	if(loaded == nullptr) {
		std::fprintf(stderr,
		             "[neurodraw::Image] Could not load image '%s', using placeholder checkerboard instead.\n",
		             path.c_str());
		return get_placeholder_texture_data();
	}

	SDL_Surface* rgba = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(loaded);
	if(rgba == nullptr) {
		std::fprintf(stderr, "[neurodraw::Image] Could not convert image '%s': %s\n",
		             path.c_str(), SDL_GetError());
		return get_placeholder_texture_data();
	}

	std::shared_ptr<TextureData> texture(new TextureData());
	texture->width = rgba->w;
	texture->height = rgba->h;
	texture->pixel_data.assign(static_cast<unsigned char*>(rgba->pixels),
	                           static_cast<unsigned char*>(rgba->pixels) + (rgba->h * rgba->pitch));
	texture->dirty = true;
	SDL_FreeSurface(rgba);

	cache[path] = texture;
	return texture;
}

bool upload_texture(TextureData& texture) {
	if(texture.dirty == false)
		return texture.texture != 0;

	if(texture.width <= 0 || texture.height <= 0 || texture.pixel_data.empty())
		return false;

	if(texture.texture == 0)
		glGenTextures(1, &texture.texture);

	glBindTexture(GL_TEXTURE_2D, texture.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D,
	             0,
	             GL_RGBA,
	             texture.width,
	             texture.height,
	             0,
	             GL_RGBA,
	             GL_UNSIGNED_BYTE,
	             texture.pixel_data.data());
	glBindTexture(GL_TEXTURE_2D, 0);

	texture.dirty = false;
	return true;
}

}  // namespace

class Image::Impl {
	public:
		Impl()
			: path_(""),
			  height_(0.30f),
			  width_(0.0f),
			  alpha_(1.0f),
			  fit_mode_(Image::FitMode::Contain),
			  texture_data_(),
			  horizontal_alignment_(Image::HorizontalAlign::Center),
			  vertical_alignment_(Image::VerticalAlign::Middle) {}

		std::string path_;
		float height_;
		float width_;
		float alpha_;
		Image::FitMode fit_mode_;
		std::shared_ptr<TextureData> texture_data_;
		Image::HorizontalAlign horizontal_alignment_;
		Image::VerticalAlign vertical_alignment_;
};

Image::Image(const std::string& path, float height)
	: impl_(new Impl()) {
	this->impl_->height_ = height;
	if(!path.empty())
		this->load(path);
}

Image::~Image(void) {}

bool Image::load(const std::string& path) {
	this->impl_->path_ = path;
	this->impl_->texture_data_ = load_texture_data(path);
	return this->impl_->texture_data_ != nullptr;
}

std::string Image::path(void) {
	return this->impl_->path_;
}

void Image::set_size(float width, float height) {
	this->set_width(width);
	this->set_height(height);
}

void Image::set_height(float height) {
	this->impl_->height_ = height > 0.0f ? height : this->impl_->height_;
}

float Image::height(void) {
	return this->impl_->height_;
}

void Image::set_width(float width) {
	this->impl_->width_ = width > 0.0f ? width : 0.0f;
}

float Image::width(void) {
	if(this->impl_->width_ > 0.0f)
		return this->impl_->width_;

	if(this->impl_->texture_data_ == nullptr)
		return 0.0f;

	if(this->impl_->texture_data_->width <= 0 || this->impl_->texture_data_->height <= 0)
		return 0.0f;

	return this->impl_->height_ *
	       (static_cast<float>(this->impl_->texture_data_->width) /
	        static_cast<float>(this->impl_->texture_data_->height));
}

float Image::intrinsic_width(void) {
	if(this->impl_->texture_data_ == nullptr)
		return 0.0f;

	return static_cast<float>(this->impl_->texture_data_->width);
}

float Image::intrinsic_height(void) {
	if(this->impl_->texture_data_ == nullptr)
		return 0.0f;

	return static_cast<float>(this->impl_->texture_data_->height);
}

void Image::set_alpha(float alpha) {
	this->impl_->alpha_ = alpha;
}

float Image::alpha(void) {
	return this->impl_->alpha_;
}

void Image::hide(void) {
	this->set_alpha(0.0f);
}

void Image::show(void) {
	this->set_alpha(1.0f);
}

void Image::set_fit_mode(FitMode mode) {
	this->impl_->fit_mode_ = mode;
}

Image::FitMode Image::fit_mode(void) {
	return this->impl_->fit_mode_;
}

void Image::set_alignment(HorizontalAlign horizontal, VerticalAlign vertical) {
	this->impl_->horizontal_alignment_ = horizontal;
	this->impl_->vertical_alignment_ = vertical;
}

Image::HorizontalAlign Image::horizontal_alignment(void) {
	return this->impl_->horizontal_alignment_;
}

Image::VerticalAlign Image::vertical_alignment(void) {
	return this->impl_->vertical_alignment_;
}

bool Image::upload_texture(void) {
	if(this->impl_->texture_data_ == nullptr)
		return false;

	return ::neurodraw::upload_texture(*this->impl_->texture_data_);
}

void Image::draw(void) {
	if(this->impl_->alpha_ <= 0.0f)
		return;

	if(this->upload_texture() == false || this->impl_->texture_data_ == nullptr)
		return;

	if(this->impl_->texture_data_->height == 0)
		return;

	float px = 0.0f;
	float py = 0.0f;
	float rot = 0.0f;
	this->current_transform(&px, &py, &rot);

	const float intrinsic_aspect =
		static_cast<float>(this->impl_->texture_data_->width) /
		static_cast<float>(this->impl_->texture_data_->height);

	float box_width = this->width();
	float box_height = this->impl_->height_;
	float draw_width = box_width;
	float draw_height = box_height;
	float tex_left = 0.0f;
	float tex_right = 1.0f;
	float tex_bottom = 1.0f;
	float tex_top = 0.0f;

	if(this->impl_->fit_mode_ == FitMode::Contain) {
		draw_width = std::min(box_width, box_height * intrinsic_aspect);
		draw_height = draw_width / intrinsic_aspect;
	} else if(this->impl_->fit_mode_ == FitMode::Cover) {
		const float box_aspect = box_width / box_height;
		draw_width = box_width;
		draw_height = box_height;

		if(intrinsic_aspect > box_aspect) {
			const float visible = box_aspect / intrinsic_aspect;
			const float extra = 1.0f - visible;
			switch(this->impl_->horizontal_alignment_) {
				case HorizontalAlign::Left:
					tex_left = 0.0f;
					tex_right = visible;
					break;
				case HorizontalAlign::Center:
					tex_left = extra * 0.5f;
					tex_right = 1.0f - extra * 0.5f;
					break;
				case HorizontalAlign::Right:
					tex_left = extra;
					tex_right = 1.0f;
					break;
			}
		} else if(intrinsic_aspect < box_aspect) {
			const float visible = intrinsic_aspect / box_aspect;
			const float extra = 1.0f - visible;
			switch(this->impl_->vertical_alignment_) {
				case VerticalAlign::Top:
					tex_top = 0.0f;
					tex_bottom = visible;
					break;
				case VerticalAlign::Middle:
					tex_top = extra * 0.5f;
					tex_bottom = 1.0f - extra * 0.5f;
					break;
				case VerticalAlign::Bottom:
					tex_top = extra;
					tex_bottom = 1.0f;
					break;
			}
		}
	}

	float left = 0.0f;
	float right = 0.0f;
	switch(this->impl_->horizontal_alignment_) {
		case HorizontalAlign::Left:
			left = 0.0f;
			right = draw_width;
			break;
		case HorizontalAlign::Center:
			left = -draw_width * 0.5f;
			right = draw_width * 0.5f;
			break;
		case HorizontalAlign::Right:
			left = -draw_width;
			right = 0.0f;
			break;
	}

	float bottom = 0.0f;
	float top = 0.0f;
	switch(this->impl_->vertical_alignment_) {
		case VerticalAlign::Top:
			top = 0.0f;
			bottom = -draw_height;
			break;
		case VerticalAlign::Middle:
			top = draw_height * 0.5f;
			bottom = -draw_height * 0.5f;
			break;
		case VerticalAlign::Bottom:
			top = draw_height;
			bottom = 0.0f;
			break;
	}

	glPushMatrix();
	glTranslatef(px, py, 0.0f);
	glRotatef(rot, 0.0f, 0.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, this->impl_->texture_data_->texture);
	glColor4f(1.0f, 1.0f, 1.0f, this->impl_->alpha_);

	glBegin(GL_QUADS);
	glTexCoord2f(tex_left,  tex_bottom); glVertex2f(left,  bottom);
	glTexCoord2f(tex_right, tex_bottom); glVertex2f(right, bottom);
	glTexCoord2f(tex_right, tex_top);    glVertex2f(right, top);
	glTexCoord2f(tex_left,  tex_top);    glVertex2f(left,  top);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void Image::local_bounds(float* min_x, float* min_y,
                         float* max_x, float* max_y) {
	float draw_width = this->width();
	float draw_height = this->height();

	if(this->impl_->fit_mode_ == FitMode::Contain &&
	   this->impl_->texture_data_ != nullptr &&
	   this->impl_->texture_data_->width > 0 &&
	   this->impl_->texture_data_->height > 0) {
		const float intrinsic_aspect =
			static_cast<float>(this->impl_->texture_data_->width) /
			static_cast<float>(this->impl_->texture_data_->height);
		draw_width = std::min(draw_width, draw_height * intrinsic_aspect);
		draw_height = draw_width / intrinsic_aspect;
	}

	float left = 0.0f;
	float right = 0.0f;
	switch(this->impl_->horizontal_alignment_) {
		case HorizontalAlign::Left:
			left = 0.0f;
			right = draw_width;
			break;
		case HorizontalAlign::Center:
			left = -draw_width * 0.5f;
			right = draw_width * 0.5f;
			break;
		case HorizontalAlign::Right:
			left = -draw_width;
			right = 0.0f;
			break;
	}

	float bottom = 0.0f;
	float top = 0.0f;
	switch(this->impl_->vertical_alignment_) {
		case VerticalAlign::Top:
			top = 0.0f;
			bottom = -draw_height;
			break;
		case VerticalAlign::Middle:
			top = draw_height * 0.5f;
			bottom = -draw_height * 0.5f;
			break;
		case VerticalAlign::Bottom:
			top = draw_height;
			bottom = 0.0f;
			break;
	}

	if(min_x != nullptr)
		*min_x = left;
	if(min_y != nullptr)
		*min_y = bottom;
	if(max_x != nullptr)
		*max_x = right;
	if(max_y != nullptr)
		*max_y = top;
}

}
