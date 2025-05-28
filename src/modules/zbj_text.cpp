// zbj_font.cpp
#include "ztype.h"
#include "zfont.h"
#include "zbj.h"
#include "zbj_text.h"
#include <iostream>

bool drawText(zbj& z, Renderer& renderer, Font font, const char* text, const FPoint& pos) {
	if (z.getTexture()) { 
        SDL_DestroyTexture(z.getTexture());
        z.getTexture() = nullptr;
    }

    if (!font || !text) { 
        std::cerr << "Error -> drawText() : Invalid font or text!\n"; 
        return false; 
    }

    size_t textLength = strlen(text);
    if (textLength == 0) { 
        std::cerr << "Error -> drawText() : Empty text string!\n"; 
        return false; 
    }

    Surface s = TTF_RenderText_Blended(font, text, textLength, z.getColor());

    if (!s) { 
        std::cerr << "Error -> drawText() : Could not render text : " << SDL_GetError() << "\n";  
        return false; 
    }

    z.getTexture() = SDL_CreateTextureFromSurface(renderer, s);

    if (!z.getTexture()) { 
        std::cerr << "Error -> drawText() : Could not create texture from text : " << SDL_GetError() << "\n"; 
        SDL_DestroySurface(s); 
        return false; 
    }

    z.getTransform().setBound( {pos.x, pos.y, (float)s->w, (float)s->h} );
    SDL_DestroySurface(s);
    return true;
}