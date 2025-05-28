// zbj.cpp
#include "zbj.h"
#include "ztype.h"
#include "zbj_rect.h"
#include "zbj_vertex.h"
#include "zbj_text.h"
#include "zbj_image.h"
#include "zbj_line.h"
#include <vector>
#include <iostream>

zbj::zbj(const Transform& transform, const Color& color) : transform(transform), color(color) {
    texture = nullptr;
}

zbj::zbj(zbj&& other) noexcept
	: transform(std::move(other.transform)),
	color(std::move(other.color)),
	texture(std::move(other.texture)) {}

zbj& zbj::operator=(zbj&& other) noexcept {
	if (this != &other) {
		transform = std::move(other.transform);
		color = std::move(other.color);
		texture = std::move(other.texture);
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
    return drawLine(*this, renderer, p1, p2);
}

bool zbj::draw(Renderer& renderer, float radiusScale) {
    return drawRect(*this, renderer, radiusScale);
}

bool zbj::draw(Renderer& renderer, Font font, const char* text, const FPoint& pos) {
    return drawText(*this, renderer, font, text, pos);
}

bool zbj::draw(Renderer& renderer, const char* path) {
    return drawImage(*this, renderer, path);
}

bool zbj::draw(Renderer& renderer, std::vector<int>& indices, std::vector<Vertex>& vertices) {
    return drawPolygon(*this, renderer, indices, vertices);
}

bool zbj::show(Renderer& renderer) {
    if (!texture) { 
        std::cerr << "Error -> zbj::show() : No texture to show\n"; 
        return false; 
    }

	if(color.a < 255){ 
		setOpacity(color.a); 
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	}
    
    const FBound b = transform.getBound();
    
    if (!SDL_RenderTexture(renderer, texture, nullptr, &b)) {
        std::cerr << "Error -> zbj::show() : Failed to render texture : " << SDL_GetError() << "\n";
        return false;
    }
    return true;
}

bool zbj::show(Renderer& renderer, FBound& srcRect) {
    if (!texture) { 
        std::cerr << "Error -> zbj::show() : No texture to show\n"; 
        return false; 
    }

	if(color.a < 255){ 
		setOpacity(color.a); 
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	}

    const FBound b = transform.getBound();
	
    if (!SDL_RenderTexture(renderer, texture, &srcRect, &b)) {
        std::cerr << "Error -> zbj::show() : Failed to render texture : " << SDL_GetError() << "\n";
        return false;
    }
    return true;
}

bool zbj::reset() {
    if (!texture) return true;
    SDL_DestroyTexture(texture);
    texture = nullptr;
    transform = Transform();
    color = {0, 0, 0, 0};
    return true;
}

bool zbj::clear(std::string_view property){
	if(property == "Texture"){
		SDL_DestroyTexture(texture);
		texture = nullptr;
		return true;
	}else if(property == "Transform"){
		transform = Transform();
		return true;
	}else if(property == "Color"){
		color = { 0, 0, 0, 255 };
		return true;
	}
	std::cerr << "Error -> zbj::clear() : Invalid Argument\n";
	return false;
}

bool zbj::setOpacity(Uint8 a){
	if(!SDL_SetTextureAlphaMod(texture, a)){
		std::cerr << "Error -> zbj::setOpacity : " << SDL_GetError() << "\n";
		return false;
	}
	return true;
}

void zbj::setTransform(const FBound& newBound) { transform.setBound(newBound); }
void zbj::setTransform(const FPoint& newPosition) { transform.setPosition(newPosition); }
void zbj::setColor(const Color& newColor) { color = newColor; }
void zbj::setAnchor(Anchor newAnchor) { transform.setAnchor(newAnchor); }
Transform& zbj::getTransform() { return transform; }
Texture& zbj::getTexture() { return texture; }
Color& zbj::getColor() { return color; }