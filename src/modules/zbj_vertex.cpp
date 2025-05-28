// zbj_vertex.cpp
#include "ztype.h"
#include <vector>
#include "zbj_vertex.h"
#include "zbj.h"
#include <iostream>

bool drawPolygon(zbj& z, Renderer& renderer, std::vector<int>& indices, std::vector<Vertex>& vertices) {
    if (z.getTexture()) { 
        SDL_DestroyTexture(z.getTexture());
        z.getTexture() = nullptr;
    }

    z.getTexture() = SDL_CreateTexture(
		renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 
		(int)z.getTransform().getBound().w, (int)z.getTransform().getBound().h
	);
                              
    if (!z.getTexture()) {
        std::cerr << "Error -> drawPolygon() : Could not create texture : " << SDL_GetError() << "\n";
        return false;
    }
    
    Texture oldTarget = SDL_GetRenderTarget(renderer);
    if (!SDL_SetRenderTarget(renderer, z.getTexture())) {
        std::cerr << "Error -> drawPolygon() : Could not set render target : " << SDL_GetError() << "\n";
        SDL_DestroyTexture(z.getTexture());
        z.getTexture() = nullptr;
        return false;
    }
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    
    if (!SDL_RenderGeometry(
		renderer, nullptr, vertices.data(), 
		vertices.size(), indices.data(), indices.size())) {
        std::cerr << "Error -> drawPolygon() : Could not draw geometry : " << SDL_GetError() << "\n";
        SDL_SetRenderTarget(renderer, oldTarget);
        SDL_DestroyTexture(z.getTexture());
        z.getTexture() = nullptr;
        return false;
    }
    
    SDL_SetRenderTarget(renderer, oldTarget);
    return true;
}