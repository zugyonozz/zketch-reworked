// zbj_image.cpp
#include "ztype.h"
#include "zbj.h"
#include "SDL_image.h"
#include "zbj_image.h"
#include <iostream>

bool drawImage(zbj& z, Renderer& renderer, const char* path) {
    if (z.getTexture()) { 
        SDL_DestroyTexture(z.getTexture());
        z.getTexture() = nullptr;
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

    z.getTexture() = SDL_CreateTextureFromSurface(renderer, s);

    if (!z.getTexture()) { 
        std::cerr << "Error -> Could not create texture from image : " << SDL_GetError() << "\n"; 
        SDL_DestroySurface(s);
        return false; 
    }

    z.getTransform().setBound({
		z.getTransform().getBound().x, z.getTransform().getBound().y, 
		(float)s->w, (float)s->h
	});

    SDL_DestroySurface(s);
    return true;
}