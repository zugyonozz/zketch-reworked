// zbj_line.cpp
#include "ztype.h"
#include "zbj.h"
#include "zbj_line.h"
#include <iostream>

bool drawLine(zbj& z, Renderer& renderer, const FPoint& p1, const FPoint& p2) {
    if (z.getTexture()) { 
        SDL_DestroyTexture(z.getTexture());
        z.getTexture() = nullptr;
    }

    z.getTexture() = SDL_CreateTexture(
		renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 
		(int)z.getTransform().getBound().w, (int)z.getTransform().getBound().h
	);

    if (!z.getTexture()) { 
        std::cerr << "Error -> Could not create texture : " << SDL_GetError() << "\n"; 
        return false; 
    }

    Texture oldTarget = SDL_GetRenderTarget(renderer);
    if (!SDL_SetRenderTarget(renderer, z.getTexture())) {
        std::cerr << "Error -> Could not set render target : " << SDL_GetError() << "\n";
        SDL_DestroyTexture(z.getTexture());
        z.getTexture() = nullptr;
        return false;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, z.getColor().r, z.getColor().g, z.getColor().b, z.getColor().a);
    SDL_RenderLine(renderer, p1.x, p1.y, p2.x, p2.y);
    SDL_SetRenderTarget(renderer, oldTarget);
    return true;
}