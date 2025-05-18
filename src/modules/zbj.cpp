#include "zbj.h"

bool zbj::draw(Renderer& renderer, FPoint p1, FPoint p2) {
	if (texture) { 
		std::cerr << "Error -> Could not draw new line : " << SDL_GetError() << "\n"; 
		return false; 
	}

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, transform.getBound().w, transform.getBound().h);

	if (!texture) { 
		std::cerr << "Error -> Could not create texture : " << SDL_GetError() << "\n"; 
		return false; 
	}

	Texture oldTarget = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderLine(renderer, p1.x, p1.y, p2.x, p2.y);
	SDL_SetRenderTarget(renderer, oldTarget);
	return true;
}

bool zbj::draw(Renderer& renderer, float radiusScale, bool fill) {
	if (texture) { 
		std::cerr << "Error -> Could not draw new rectangle : " << SDL_GetError() << "\n"; 
		return false; 
	}

	if(!fill){
		Texture t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, transform.getBound().w, transform.getBound().h);
		SDL_Texture* prevTarget = SDL_GetRenderTarget(renderer);
		SDL_SetRenderTarget(renderer, t);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderRect(renderer, &transform.getBound());
		SDL_SetRenderTarget(renderer, prevTarget);
		texture = t;
		return;
	}

	if (radiusScale < 0.0f || radiusScale > 1.0f) { 
		std::cerr << "Error -> Radius scale value must be between 0.0 and 1.0" << "\n"; 
		return false; 
	}

	if (radiusScale == 0.0f) {
		Surface s = SDL_CreateSurface(transform.getBound().w, transform.getBound().h, SDL_PIXELFORMAT_RGBA32);
		if (!s) { std::cerr << "Error -> Could not create surface : " << SDL_GetError() << "\n"; return false; }   
		Uint32 pixel = SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, color.r, color.g, color.b, color.a);
		SDL_FillSurfaceRect(s, nullptr, pixel);
		texture = SDL_CreateTextureFromSurface(renderer, s);
		SDL_DestroySurface(s);

		if (!texture) { 
			std::cerr << "Error -> Could not create texture : " << SDL_GetError() << "\n"; 
			return false; 
		}
		return true;
	}

	float minDimension = std::min(transform.getBound().w, transform.getBound().h);
	float radius = minDimension * radiusScale * 0.5f;
	Surface s = SDL_CreateSurface(transform.getBound().w, transform.getBound().h, SDL_PIXELFORMAT_RGBA32);

	if (!s) { 
		std::cerr << "Error -> Could not create surface : " << SDL_GetError() << "\n"; 
		return false; 
	}

	SDL_FillSurfaceRect(s, nullptr, SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, 0, 0, 0, 0));

	if (!SDL_LockSurface(s)) { 
		std::cerr << "Error -> Failed to lock surface : " << SDL_GetError() << "\n"; SDL_DestroySurface(s); 
		return false; 
	
	}
	Uint32 pixel = SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, color.r, color.g, color.b, color.a);
	SDL_Rect inner1 = { static_cast<int>(radius), 0, transform.getBound().w - static_cast<int>(2 * radius), transform.getBound().h };
	SDL_Rect inner2 = { 0, static_cast<int>(radius), transform.getBound().w, transform.getBound().h - static_cast<int>(2 * radius) };
	SDL_FillSurfaceRect(s, &inner1, pixel);
	SDL_FillSurfaceRect(s, &inner2, pixel);
	auto drawCorner = [&](SDL_FPoint center, float startAngle) {
		for (float angle = 0; angle <= 90.0f; angle += 0.5f) {
			float rad = (startAngle + angle) * (M_PI / 180.0f);
			for (float r = 0; r <= radius; r += 0.5f) {
				int x = static_cast<int>(center.x + r * cosf(rad));
				int y = static_cast<int>(center.y + r * sinf(rad));
				if (x >= 0 && y >= 0 && x < s->w && y < s->h) {
					static_cast<Uint32*>(s->pixels)[y * s->w + x] = pixel;
				}
			}
		}
	};
	drawCorner({ radius, radius }, 180.0f);
	drawCorner({ static_cast<float>(transform.getBound().w) - radius, radius }, 270.0f);
	drawCorner({ static_cast<float>(transform.getBound().w) - radius, static_cast<float>(transform.getBound().h) - radius }, 0.0f);
	drawCorner({ radius, static_cast<float>(transform.getBound().h) - radius }, 90.0f);
	SDL_UnlockSurface(s);
	texture = SDL_CreateTextureFromSurface(renderer, s);
	SDL_DestroySurface(s);

	if (!texture) { 
		std::cerr << "Error -> Could not create rounded texture : " << SDL_GetError() << "\n"; 
		return false; 
	}
	return true;
}

bool zbj::draw(Renderer& renderer, Font font, const char* text, FPoint pos) {
	if (texture) { 
		std::cerr << "Error -> Could not draw new text : " << SDL_GetError() << "\n"; 
		return false; 
	}

	if (!font || !text) { 
		std::cerr << "Error -> Invalid font or text!\n" << SDL_GetError(); 
		return false; 
	}

	size_t textLength = strlen(text);
	if (textLength == 0) { 
		std::cerr << "Error -> Empty text string!\n"; 
		return false; 
	}

	Surface s = TTF_RenderText_Blended(font, text, textLength, color);

	if (!s) { 
		std::cerr << "Error -> Could not render text : " << SDL_GetError() << "\n";  
		return false; 
	}

	texture = SDL_CreateTextureFromSurface(renderer, s);

	if (!texture) { 
		std::cerr << "Error -> Could not create texture from text : " << SDL_GetError() << "\n"; 
		SDL_DestroySurface(s); 
		return false; 
	}

	transform.setBound({pos.x, pos.y, (float)s->w, (float)s->h});
	SDL_DestroySurface(s);
	return true;
}

bool zbj::draw(Renderer& renderer, const char* path) {
	if (texture) { 
		std::cerr << "Error -> Could not draw new image : " << SDL_GetError() << "\n"; 
		return false; 
	}

	if (!path) {
		std::cerr << "Error -> Invalid image path!\n"; 
		return false; 
	}

	Surface s = IMG_Load(path);

	if (!s) { 
		std::cerr << "Error -> Could not load image : " << SDL_GetError() << "\n"; 
		return false; 
	}

	texture = SDL_CreateTextureFromSurface(renderer, s);

	if (!texture) { 
		std::cerr << "Error -> Could not create texture from image : " << SDL_GetError() << "\n"; 
		SDL_DestroySurface(s);
		return false; 
	}

	transform.setBound({transform.getBound().x, transform.getBound().y, (float)s->w, (float)s->h});
	SDL_DestroySurface(s);
	return true;
}

bool zbj::draw(Renderer& renderer, std::vector<int>& indices, std::vector<Vertex>& vertices){
	if (texture) { 
		std::cerr << "Error -> Could not draw new image : " << SDL_GetError() << "\n"; 
		return false; 
	}

	if(!SDL_RenderGeometry(renderer, texture, vertices.data(), vertices.size(), indices.data(), indices.size())){
		std::cerr << "Error -> Could not draw geometry : " << SDL_GetError() << "\n"; 
		return false; 
	}
	return true;
}

zbj::zbj(Transform Transform, Color color) : color(color), transform(transform) {
	this->texture = nullptr;
}

zbj::~zbj() {
	if(texture){ SDL_DestroyTexture(texture); }
}

bool zbj::show(Renderer& renderer) {
	if (!texture) { 
		std::cerr << "Error -> No texture show : " << SDL_GetError() << "\n"; 
		return false; 
	}
	
	SDL_RenderTexture(renderer, texture, nullptr, &transform.getBound());
	return true;
}

bool zbj::clear(){
	if(!texture){
		std::cerr << "Error -> No texture show : " << SDL_GetError() << "\n"; 
		return false; 
	}
	transform = {};
	color = {};
	SDL_DestroyTexture(texture);
	return true;
}

void zbj::setTransform(const FBound& newBound) { 
	transform.setBound(newBound);
}

void zbj::setTransform(const FPoint& newPosition){
	transform.setPosition(newPosition);
}

void zbj::setColor(const Color newColor) { 
	color = newColor; 
}

void zbj::setAnchor(Anchor& newAnchor) {
	transform.setAnchor(newAnchor);
}

Transform& zbj::getTransform(){
	return transform;
}

const Texture& zbj::getTexture() const{
	return texture;
}

const Color& zbj::getColor() const{
	return getColor();
}