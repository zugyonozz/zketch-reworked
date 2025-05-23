#include "zbj.h"

zbj::zbj(const Transform& transform, const Color& color) : transform(transform), color(color) {
    texture = nullptr;
}

zbj::zbj(zbj&& other) noexcept
	: transform(std::move(other.transform)),
	color(std::move(other.color)),
	texture(std::move(other.texture)) {
		
	}

zbj& zbj::operator=(zbj&& other) noexcept {
	if (this != &other) {
		transform = std::move(other.transform);
		color = std::move(other.color);
		texture = std::move(other.texture);
		// Reset other kalau perlu
	}
	return *this;
}

zbj::~zbj() {
    if(texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

bool zbj::draw(Renderer& renderer, const FPoint& p1, const FPoint& p2) {
    if (texture) { 
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 
                               static_cast<int>(transform.getBound().w), 
                               static_cast<int>(transform.getBound().h));

    if (!texture) { 
        std::cerr << "Error -> Could not create texture : " << SDL_GetError() << "\n"; 
        return false; 
    }

    Texture oldTarget = SDL_GetRenderTarget(renderer);
    if (!SDL_SetRenderTarget(renderer, texture)) {
        std::cerr << "Error -> Could not set render target : " << SDL_GetError() << "\n";
        SDL_DestroyTexture(texture);
        texture = nullptr;
        return false;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderLine(renderer, p1.x, p1.y, p2.x, p2.y);
    SDL_SetRenderTarget(renderer, oldTarget);
    return true;
}

bool zbj::draw(Renderer& renderer, float radiusScale) {
    if (texture) { 
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    if (radiusScale < 0.0f || radiusScale > 1.0f) { 
        std::cerr << "Error -> Radius scale value must be between 0.0 and 1.0" << "\n"; 
        return false; 
    }

    if (radiusScale == 0.0f) {
        Surface s = SDL_CreateSurface(static_cast<int>(transform.getBound().w), 
                                     static_cast<int>(transform.getBound().h), 
                                     SDL_PIXELFORMAT_RGBA32);
                                     
        if (!s) { 
            std::cerr << "Error -> Could not create surface : " << SDL_GetError() << "\n"; 
            return false; 
        }
        
        const SDL_PixelFormatDetails* format = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32);
        if (!format) {
            std::cerr << "Error -> Could not get pixel format details : " << SDL_GetError() << "\n";
            SDL_DestroySurface(s);
            return false;
        }
        
        Uint32 pixel = SDL_MapRGBA(format, nullptr, color.r, color.g, color.b, color.a);
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
    Surface s = SDL_CreateSurface(static_cast<int>(transform.getBound().w), 
                                 static_cast<int>(transform.getBound().h), 
                                 SDL_PIXELFORMAT_RGBA32);

    if (!s) { 
        std::cerr << "Error -> Could not create surface : " << SDL_GetError() << "\n"; 
        return false; 
    }

    const SDL_PixelFormatDetails* format = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32);
    if (!format) {
        std::cerr << "Error -> Could not get pixel format details : " << SDL_GetError() << "\n";
        SDL_DestroySurface(s);
        return false;
    }
    
    SDL_FillSurfaceRect(s, nullptr, SDL_MapRGBA(format, nullptr, 0, 0, 0, 0));

    if (!SDL_LockSurface(s)) { 
        std::cerr << "Error -> Failed to lock surface : " << SDL_GetError() << "\n"; 
        SDL_DestroySurface(s); 
        return false; 
    }
    
    Uint32 pixel = SDL_MapRGBA(format, nullptr, color.r, color.g, color.b, color.a);
    
    SDL_Rect inner1 = { 
        static_cast<int>(radius), 
        0, 
        static_cast<int>(transform.getBound().w - 2 * radius), 
        static_cast<int>(transform.getBound().h) 
    };
    
    SDL_Rect inner2 = { 
        0, 
        static_cast<int>(radius), 
        static_cast<int>(transform.getBound().w), 
        static_cast<int>(transform.getBound().h - 2 * radius) 
    };
    
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
    
    drawCorner({radius, radius}, 180.0f);
    drawCorner({static_cast<float>(transform.getBound().w) - radius, radius}, 270.0f);
    drawCorner({static_cast<float>(transform.getBound().w) - radius, static_cast<float>(transform.getBound().h) - radius}, 0.0f);
    drawCorner({radius, static_cast<float>(transform.getBound().h) - radius}, 90.0f);
    
    SDL_UnlockSurface(s);
    texture = SDL_CreateTextureFromSurface(renderer, s);
    SDL_DestroySurface(s);

    if (!texture) { 
        std::cerr << "Error -> Could not create rounded texture : " << SDL_GetError() << "\n"; 
        return false; 
    }
    return true;
}

bool zbj::draw(Renderer& renderer, Font font, const char* text, const FPoint& pos) {
    if (texture) { 
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    if (!font || !text) { 
        std::cerr << "Error -> Invalid font or text!\n"; 
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

    transform.setBound({pos.x, pos.y, static_cast<float>(s->w), static_cast<float>(s->h)});
    SDL_DestroySurface(s);
    return true;
}

bool zbj::draw(Renderer& renderer, const char* path) {
    if (texture) { 
        SDL_DestroyTexture(texture);
        texture = nullptr;
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

    transform.setBound({transform.getBound().x, transform.getBound().y, 
                       static_cast<float>(s->w), static_cast<float>(s->h)});
    SDL_DestroySurface(s);
    return true;
}

bool zbj::draw(Renderer& renderer, std::vector<int>& indices, std::vector<Vertex>& vertices) {
    if (texture) { 
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                              static_cast<int>(transform.getBound().w),
                              static_cast<int>(transform.getBound().h));
                              
    if (!texture) {
        std::cerr << "Error -> Could not create texture : " << SDL_GetError() << "\n";
        return false;
    }
    
    Texture oldTarget = SDL_GetRenderTarget(renderer);
    if (!SDL_SetRenderTarget(renderer, texture)) {
        std::cerr << "Error -> Could not set render target : " << SDL_GetError() << "\n";
        SDL_DestroyTexture(texture);
        texture = nullptr;
        return false;
    }
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    
    if (!SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(), 
                         indices.data(), indices.size())) {
        std::cerr << "Error -> Could not draw geometry : " << SDL_GetError() << "\n";
        SDL_SetRenderTarget(renderer, oldTarget);
        SDL_DestroyTexture(texture);
        texture = nullptr;
        return false;
    }
    
    SDL_SetRenderTarget(renderer, oldTarget);
    return true;
}

bool zbj::show(Renderer& renderer) {
	if(color.a < 255){ 
		setOpacity(color.a); 
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	}
    if (!texture) { 
        std::cerr << "Error -> No texture to show\n"; 
        return false; 
    }
    
    const FBound b = transform.getBound();
    
    if (!SDL_RenderTexture(renderer, texture, nullptr, &b)) {
		std::cerr << "Error -> Bound : {" << b.x << ", " << b.y << ", " << b.w << ", " << b.h << "}\n" ;
        std::cerr << "Error -> Failed to render texture : " << SDL_GetError() << "\n";
        return false;
    }
    return true;
}

bool zbj::show(Renderer& renderer, FBound& srcRect) {
	if(color.a < 255){ setOpacity(color.a); }
    if (!texture) { 
        std::cerr << "Error -> No texture to show\n"; 
        return false; 
    }
    
    const FBound b = transform.getBound();
    
    if (!SDL_RenderTexture(renderer, texture, &srcRect, &b)) {
		std::cerr << "Error -> Bound : {" << b.x << ", " << b.y << ", " << b.w << ", " << b.h << "}\n" ;
        std::cerr << "Error -> Failed to render texture : " << SDL_GetError() << "\n";
        return false;
    }
    return true;
}

bool zbj::clear() {
    if (!texture) {
        return true; // Already cleared
    }
    
    SDL_DestroyTexture(texture);
    texture = nullptr;
    transform = Transform();
    color = {0, 0, 0, 0};
    return true;
}

bool zbj::setOpacity(Uint8 a){
	if(!SDL_SetTextureAlphaMod(texture, a)){
		std::cerr << "Error -> zbj::setOpacity : " << SDL_GetError() << "\n";
		return false;
	}
	return true;
}

void zbj::setTransform(const FBound& newBound) { 
    transform.setBound(newBound);
}

void zbj::setTransform(const FPoint& newPosition) {
    transform.setPosition(newPosition);
}

void zbj::setColor(const Color& newColor) { 
    color = newColor; 
}

void zbj::setAnchor(Anchor newAnchor) {
    transform.setAnchor(newAnchor);
}

Transform& zbj::getTransform() {
    return transform;
}

const Texture& zbj::getTexture() const {
    return texture;
}

const Color& zbj::getColor() const {
    return color;
}