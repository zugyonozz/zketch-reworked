// zbj_rect.cpp
#include "ztype.h"
#include <vector>
#include "zbj_vertex.h"
#include "zbj.h"
#include "utils.h"
#include "zbj_rect.h"
#include <cmath>
#include <iostream>

void AddQuarterCircle(
	zbj& z,
	std::vector<Vertex>& vtx, 
	std::vector<int>& idx, 
	float cx, float cy, 
	float r, int seg, 
	float startDeg) {
    int base = vtx.size();
    
    vtx.push_back({{cx, cy}, Color2FColor(z.getColor()), {}});

    for (int i = 0; i <= seg; ++i) {
        float theta = (startDeg + (90.0f * i / seg)) * M_PI / 180.0f;
        float x = cx + r * cosf(theta);
        float y = cy + r * sinf(theta);
        vtx.push_back({{x, y}, Color2FColor(z.getColor()), {}});
    }

    for (int i = 0; i < seg; ++i) {
        idx.push_back(base);        // center
        idx.push_back(base + i + 1); // current arc point
        idx.push_back(base + i + 2); // next arc point
    }
}

void AddQuad(
	zbj& z,
	std::vector<Vertex>& vtx, 
	std::vector<int>& idx, 
	SDL_FPoint a, SDL_FPoint b, 
	SDL_FPoint c, SDL_FPoint d) {
    int base = vtx.size();
    vtx.push_back({a, Color2FColor(z.getColor()), {}});
    vtx.push_back({b, Color2FColor(z.getColor()), {}});
    vtx.push_back({c, Color2FColor(z.getColor()), {}});
    vtx.push_back({d, Color2FColor(z.getColor()), {}});

    idx.insert(idx.end(), { base, base + 1, base + 2, base, base + 2, base + 3 });
}

bool RoundedBox(zbj& z, Renderer renderer, float r) {
    if (r > 1.0f || r <= 0.0f) return false;
    
    FBound bounds = z.getTransform().getBound();
    if (bounds.w <= 0.0f || bounds.h <= 0.0f) return false;

    float width = bounds.w, height = bounds.h, x = 0.0f, y = 0.0f;
    float radius = std::min(width, height) * r * 0.4f;
    int seg = std::max(8, int(radius * 0.5f));

    std::vector<Vertex> vtx;
    std::vector<int> idx;

    float cornerX1 = x + radius, cornerY1 = y + radius, 
	cornerX2 = x + width - radius, cornerY2 = y + height - radius;

    if (cornerX2 < cornerX1) cornerX1 = cornerX2 = width * 0.5f;
    if (cornerY2 < cornerY1) cornerY1 = cornerY2 = height * 0.5f;
    if (width > 2 * radius) {
        AddQuad(z, vtx, idx,
            SDL_FPoint{cornerX1, y},
            SDL_FPoint{cornerX2, y},
            SDL_FPoint{cornerX2, cornerY1},
            SDL_FPoint{cornerX1, cornerY1});

        AddQuad(z, vtx, idx,
            SDL_FPoint{cornerX1, cornerY2},
            SDL_FPoint{cornerX2, cornerY2},
            SDL_FPoint{cornerX2, y + height},
            SDL_FPoint{cornerX1, y + height});
    }
    if (height > 2 * radius) {
        AddQuad(z, vtx, idx,
            SDL_FPoint{x, cornerY1},
            SDL_FPoint{cornerX1, cornerY1},
            SDL_FPoint{cornerX1, cornerY2},
            SDL_FPoint{x, cornerY2}
		);
        AddQuad(z, vtx, idx,
            SDL_FPoint{cornerX2, cornerY1},
            SDL_FPoint{x + width, cornerY1},
            SDL_FPoint{x + width, cornerY2},
            SDL_FPoint{cornerX2, cornerY2}
		);
    }
    if (width > 2 * radius && height > 2 * radius) {
        AddQuad(z, vtx, idx,
            SDL_FPoint{cornerX1, cornerY1},
            SDL_FPoint{cornerX2, cornerY1},
            SDL_FPoint{cornerX2, cornerY2},
            SDL_FPoint{cornerX1, cornerY2}
		);
    }
    AddQuarterCircle(z, vtx, idx, cornerX1, cornerY1, radius, seg, 180);
    AddQuarterCircle(z, vtx, idx, cornerX2, cornerY1, radius, seg, 270);
    AddQuarterCircle(z, vtx, idx, cornerX2, cornerY2, radius, seg, 0);
    AddQuarterCircle(z, vtx, idx, cornerX1, cornerY2, radius, seg, 90);
    return drawPolygon(z, renderer, idx, vtx);
}

bool drawRect(zbj& z, Renderer& renderer, float r){
	if (z.getTexture()) { 
        SDL_DestroyTexture(z.getTexture());
        z.getTexture() = nullptr;
    }

    if (r < 0.0f || r > 1.0f) { 
        std::cerr << "Error -> drawRect() : Radius scale value must be between 0.0 and 1.0" << "\n"; 
        return false; 
    }

    if (r == 0.0f) {
        Surface s = SDL_CreateSurface(
			(int)z.getTransform().getBound().w, 
			(int)z.getTransform().getBound().h, 
			SDL_PIXELFORMAT_RGBA32
		);
                                     
        if (!s) { 
            std::cerr << "Error -> drawRect() : Could not create surface : " << SDL_GetError() << "\n"; 
            return false; 
        }
        
        const SDL_PixelFormatDetails* format = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32);
        if (!format) {
            std::cerr << "Error -> drawRect() : Could not get pixel format details : " << SDL_GetError() << "\n";
            SDL_DestroySurface(s);
            return false;
        }
        
        Uint32 pixel = SDL_MapRGBA(format, nullptr, z.getColor().r, z.getColor().g, z.getColor().b, z.getColor().a);
        SDL_FillSurfaceRect(s, nullptr, pixel);
        z.getTexture() = SDL_CreateTextureFromSurface(renderer, s);
        SDL_DestroySurface(s);

        if (!z.getTexture()) { 
            std::cerr << "Error -> drawRect() : Could not create texture : " << SDL_GetError() << "\n"; 
            return false; 
        }
        return true;
    }
	return RoundedBox(z, renderer, r);
}