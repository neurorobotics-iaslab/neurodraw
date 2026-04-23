#include "neurodraw/Text.h"

#include <algorithm>
#include <cstdio>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>

namespace neurodraw {

namespace {

struct TtfGuard {
	TtfGuard() {
		if(TTF_WasInit() == 0)
			TTF_Init();
	}

	~TtfGuard() = default;
};

static TtfGuard g_ttf_guard;

struct FontHandle {
	explicit FontHandle(TTF_Font* font) : font(font) {}
	~FontHandle() {
		if(font != nullptr)
			TTF_CloseFont(font);
	}

	TTF_Font* font;
};

std::shared_ptr<FontHandle> load_font_cached(const std::string& path, int size) {
	static std::map<std::string, std::weak_ptr<FontHandle> > cache;

	const std::string key = path + "#" + std::to_string(size);
	const auto it = cache.find(key);
	if(it != cache.end()) {
		std::shared_ptr<FontHandle> cached = it->second.lock();
		if(cached != nullptr)
			return cached;
	}

	TTF_Font* raw_font = TTF_OpenFont(path.c_str(), size);
	if(raw_font == nullptr)
		return nullptr;

	std::shared_ptr<FontHandle> font(new FontHandle(raw_font));
	cache[key] = font;
	return font;
}

std::vector<std::string> split_lines(const std::string& text) {
	std::vector<std::string> lines;
	std::stringstream stream(text);
	std::string line;

	while(std::getline(stream, line))
		lines.push_back(line);

	if(lines.empty())
		lines.push_back(text);

	return lines;
}

SDL_Surface* render_text_surface(TTF_Font* font,
                                 const std::string& text,
                                 SDL_Color color,
                                 int wrap_width,
                                 float line_spacing) {
	if(font == nullptr)
		return nullptr;

	TTF_SetFontWrappedAlign(font, TTF_WRAPPED_ALIGN_LEFT);

	std::vector<std::string> lines = split_lines(text);
	std::vector<SDL_Surface*> rendered_lines;
	int max_width = 0;
	int total_height = 0;
	const int base_lineskip = TTF_FontLineSkip(font);
	const int extra_spacing = static_cast<int>((line_spacing - 1.0f) * base_lineskip);

	for(std::size_t i = 0; i < lines.size(); ++i) {
		const std::string& line = lines.at(i).empty() ? std::string(" ") : lines.at(i);
		SDL_Surface* line_surface = nullptr;
		if(wrap_width > 0) {
			line_surface = TTF_RenderUTF8_Blended_Wrapped(font,
			                                              line.c_str(),
			                                              color,
			                                              static_cast<Uint32>(wrap_width));
		} else {
			line_surface = TTF_RenderUTF8_Blended(font, line.c_str(), color);
		}

		if(line_surface == nullptr) {
			for(std::size_t j = 0; j < rendered_lines.size(); ++j)
				SDL_FreeSurface(rendered_lines.at(j));
			return nullptr;
		}

		rendered_lines.push_back(line_surface);
		max_width = std::max(max_width, line_surface->w);
		total_height += line_surface->h;
		if(i + 1 < lines.size())
			total_height += extra_spacing;
	}

	SDL_Surface* rendered = SDL_CreateRGBSurfaceWithFormat(0,
	                                                       max_width,
	                                                       total_height,
	                                                       32,
	                                                       SDL_PIXELFORMAT_RGBA32);
	if(rendered == nullptr) {
		for(std::size_t j = 0; j < rendered_lines.size(); ++j)
			SDL_FreeSurface(rendered_lines.at(j));
		return nullptr;
	}

	SDL_FillRect(rendered, nullptr, SDL_MapRGBA(rendered->format, 0, 0, 0, 0));

	int y_offset = 0;
	for(std::size_t i = 0; i < rendered_lines.size(); ++i) {
		SDL_Surface* line_surface = rendered_lines.at(i);
		SDL_Rect dst;
		dst.x = 0;
		dst.y = y_offset;
		dst.w = line_surface->w;
		dst.h = line_surface->h;
		SDL_BlitSurface(line_surface, nullptr, rendered, &dst);
		y_offset += line_surface->h;
		if(i + 1 < lines.size())
			y_offset += extra_spacing;
		SDL_FreeSurface(line_surface);
	}

	SDL_Surface* rgba = SDL_ConvertSurfaceFormat(rendered, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(rendered);
	return rgba;
}

}  // namespace

class Text::Impl {
	public:
		Impl()
			: text_(""),
			  font_path_(""),
			  font_size_(48),
			  height_(0.15f),
			  color_(Palette::white),
			  alpha_(1.0f),
			  texture_(0),
			  texture_width_(0),
			  texture_height_(0),
			  horizontal_alignment_(Text::HorizontalAlign::Center),
			  vertical_alignment_(Text::VerticalAlign::Middle),
			  wrap_width_(0),
			  line_spacing_(1.0f),
			  dirty_(true) {}

		std::string text_;
		std::string font_path_;
		int font_size_;
		float height_;
		Color color_;
		float alpha_;
		unsigned int texture_;
		int texture_width_;
		int texture_height_;
		Text::HorizontalAlign horizontal_alignment_;
		Text::VerticalAlign vertical_alignment_;
		int wrap_width_;
		float line_spacing_;
		bool dirty_;
};

Text::Text(const std::string& text,
		   const std::string& font_path,
		   float height,
		   int font_size,
		   Color color)
	: impl_(new Impl()) {
	this->impl_->text_ = text;
	this->impl_->font_path_ = font_path;
	this->impl_->height_ = height;
	this->impl_->font_size_ = font_size;
	this->impl_->color_ = color;
}

Text::~Text(void) {
	if(this->impl_->texture_ != 0)
		glDeleteTextures(1, &this->impl_->texture_);
}

void Text::set_text(const std::string& text) {
	this->impl_->text_ = text;
	this->impl_->dirty_ = true;
}

std::string Text::text(void) {
	return this->impl_->text_;
}

void Text::set_font(const std::string& font_path, int font_size) {
	this->impl_->font_path_ = font_path;
	this->impl_->font_size_ = font_size;
	this->impl_->dirty_ = true;
}

std::string Text::font(void) {
	return this->impl_->font_path_;
}

int Text::font_size(void) {
	return this->impl_->font_size_;
}

void Text::set_height(float height) {
	this->impl_->height_ = (height > 0.0f) ? height : this->impl_->height_;
}

float Text::height(void) {
	return this->impl_->height_;
}

float Text::width(void) {
	if(this->impl_->dirty_ && !this->impl_->text_.empty() && !this->impl_->font_path_.empty()) {
		std::shared_ptr<FontHandle> font = load_font_cached(this->impl_->font_path_, this->impl_->font_size_);
		if(font != nullptr && font->font != nullptr) {
			SDL_Color color = {
				static_cast<Uint8>(this->impl_->color_.at(0)),
				static_cast<Uint8>(this->impl_->color_.at(1)),
				static_cast<Uint8>(this->impl_->color_.at(2)),
				static_cast<Uint8>(255)
			};
			SDL_Surface* rgba = render_text_surface(font->font,
			                                        this->impl_->text_,
			                                        color,
			                                        this->impl_->wrap_width_,
			                                        this->impl_->line_spacing_);
			if(rgba != nullptr) {
				this->impl_->texture_width_ = rgba->w;
				this->impl_->texture_height_ = rgba->h;
				SDL_FreeSurface(rgba);
			}
		}
	}

	if(this->impl_->texture_height_ <= 0 || this->impl_->texture_width_ <= 0)
		return 0.0f;

	return this->impl_->height_ *
	       (static_cast<float>(this->impl_->texture_width_) /
	        static_cast<float>(this->impl_->texture_height_));
}

void Text::set_color(Color color) {
	this->impl_->color_ = color;
	this->impl_->dirty_ = true;
}

void Text::set_alpha(float alpha) {
	this->impl_->alpha_ = alpha;
}

float Text::alpha(void) {
	return this->impl_->alpha_;
}

void Text::hide(void) {
	this->set_alpha(0.0f);
}

void Text::show(void) {
	this->set_alpha(1.0f);
}

void Text::set_alignment(HorizontalAlign horizontal, VerticalAlign vertical) {
	this->impl_->horizontal_alignment_ = horizontal;
	this->impl_->vertical_alignment_ = vertical;
}

Text::HorizontalAlign Text::horizontal_alignment(void) {
	return this->impl_->horizontal_alignment_;
}

Text::VerticalAlign Text::vertical_alignment(void) {
	return this->impl_->vertical_alignment_;
}

void Text::set_wrap_width(int pixels) {
	this->impl_->wrap_width_ = pixels > 0 ? pixels : 0;
	this->impl_->dirty_ = true;
}

int Text::wrap_width(void) {
	return this->impl_->wrap_width_;
}

void Text::set_line_spacing(float multiplier) {
	this->impl_->line_spacing_ = multiplier > 0.0f ? multiplier : 1.0f;
	this->impl_->dirty_ = true;
}

float Text::line_spacing(void) {
	return this->impl_->line_spacing_;
}

void Text::draw(void) {
	if(this->impl_->alpha_ <= 0.0f)
		return;

	if(this->impl_->text_.empty() || this->impl_->font_path_.empty())
		return;

	if(this->impl_->dirty_) {
		std::shared_ptr<FontHandle> font = load_font_cached(this->impl_->font_path_, this->impl_->font_size_);
		if(font == nullptr || font->font == nullptr) {
			std::fprintf(stderr, "[neurodraw::Text] Could not load font '%s': %s\n",
			             this->impl_->font_path_.c_str(), TTF_GetError());
			return;
		}

		SDL_Color color = {
			static_cast<Uint8>(this->impl_->color_.at(0)),
			static_cast<Uint8>(this->impl_->color_.at(1)),
			static_cast<Uint8>(this->impl_->color_.at(2)),
			static_cast<Uint8>(255)
		};

		SDL_Surface* rgba = render_text_surface(font->font,
		                                        this->impl_->text_,
		                                        color,
		                                        this->impl_->wrap_width_,
		                                        this->impl_->line_spacing_);
		if(rgba == nullptr) {
			std::fprintf(stderr, "[neurodraw::Text] Could not render text: %s\n", TTF_GetError());
			return;
		}

		if(this->impl_->texture_ == 0)
			glGenTextures(1, &this->impl_->texture_);

		glBindTexture(GL_TEXTURE_2D, this->impl_->texture_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D,
		             0,
		             GL_RGBA,
		             rgba->w,
		             rgba->h,
		             0,
		             GL_RGBA,
		             GL_UNSIGNED_BYTE,
		             rgba->pixels);

		this->impl_->texture_width_ = rgba->w;
		this->impl_->texture_height_ = rgba->h;
		this->impl_->dirty_ = false;

		SDL_FreeSurface(rgba);
	}

	if(this->impl_->texture_ == 0 || this->impl_->texture_height_ == 0)
		return;

	float px = 0.0f;
	float py = 0.0f;
	float rot = 0.0f;
	this->current_transform(&px, &py, &rot);

	float aspect = static_cast<float>(this->impl_->texture_width_) /
	               static_cast<float>(this->impl_->texture_height_);
	float draw_height = this->impl_->height_;
	float draw_width = draw_height * aspect;

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

	glBindTexture(GL_TEXTURE_2D, this->impl_->texture_);
	glColor4f(1.0f, 1.0f, 1.0f, this->impl_->alpha_);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(left,  bottom);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(right, bottom);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(right, top);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(left,  top);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void Text::local_bounds(float* min_x, float* min_y,
                        float* max_x, float* max_y) {
	float left = 0.0f;
	float right = this->width();
	float top = this->height();
	float bottom = 0.0f;

	switch(this->impl_->horizontal_alignment_) {
		case HorizontalAlign::Left:
			left = 0.0f;
			right = this->width();
			break;
		case HorizontalAlign::Center:
			left = -this->width() * 0.5f;
			right = this->width() * 0.5f;
			break;
		case HorizontalAlign::Right:
			left = -this->width();
			right = 0.0f;
			break;
	}

	switch(this->impl_->vertical_alignment_) {
		case VerticalAlign::Top:
			top = 0.0f;
			bottom = -this->height();
			break;
		case VerticalAlign::Middle:
			top = this->height() * 0.5f;
			bottom = -this->height() * 0.5f;
			break;
		case VerticalAlign::Bottom:
			top = this->height();
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
