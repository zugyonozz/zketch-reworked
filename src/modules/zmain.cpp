// zmain.cpp
#include "zmain.h"
#include "SDL_image.h"
#include <iostream>

zmain::zmain() : window(nullptr), renderer(nullptr) {}

zmain::zmain(const char* title, int W, int H, windowFlags flags) {
	if(createWindow(title, W, H, flags)){
		if (init()) {
			createRenderer(window);
		}
	}
}

zmain::zmain(const char* title, int W, int H, windowFlags flags, const char* driver) {
	if(createWindow(title, W, H, flags)){
		if (init()) {
			createRenderer(window, driver);
		}
	}
}

bool zmain::createWindow(const char* title, int W, int H, windowFlags flags) {
    window = SDL_CreateWindow(title, W, H, flags);
    if (!window) {
        std::cerr << "Error -> zmain::createWindow() Could not create window: " << SDL_GetError() << "\n";
        return false;
    }
    return true;
}

bool zmain::createRenderer(Window window, const char* driver) {
    renderer = SDL_CreateRenderer(window, driver);
    if (!renderer) {
        std::cerr << "Error -> zmain::createRenderer() : Could not create renderer " << SDL_GetError() << "\n";
        return false;
    }
    return true;
}

bool zmain::init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Error -> zmain::init() : Could not initialize video " << SDL_GetError() << "\n";
        return false;
    }
    return true;
}

bool zmain::present() {
    return SDL_RenderPresent(renderer);
}

const FPoint zmain::getWinSize() const {
    Point p{0, 0};
    if (window) {
        SDL_GetWindowSize(window, &p.x, &p.y);
    }
    return {(float)p.x, (float)p.y};
}

Renderer& zmain::getRenderer() {
    return renderer;
}

bool zmain::clearRender(const Color& color) {
    if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
        std::cerr << "Error -> zmain::clearRender() : Could not set color: " << SDL_GetError() << "\n";
        return false;
    }
    
    if (!SDL_RenderClear(renderer)) {
        std::cerr << "Error -> zmain::clearRender() : Could not render clear: " << SDL_GetError() << "\n";
        return false;
    }
    
    return true;
}

void zmain::delay(Uint32 ms) {
    SDL_Delay(ms);
}

bool zmain::setWindowIcon(const char* path) {
    if (!path) {
        return false;
    }
    
    SDL_Surface* icon = IMG_Load(path);
    if (!icon) {
        std::cerr << "Error -> zmain::setWindowIcon() : Could not load icon: " << SDL_GetError() << "\n";
        return false;
    }
    
    SDL_SetWindowIcon(window, icon);
    SDL_DestroySurface(icon);
    return true;
}

bool zmain::setWindowMinSize(const int& W, const int& H){
	if(!SDL_SetWindowMinimumSize(window, W, H)){
		std::cerr << "Error : zmain::setWindowMinSize() failed!\n";
		return false;
	}
	return true;
}

bool zmain::startTextInput() {
    if (!SDL_StartTextInput(window)) {
        std::cerr << "Error: zmain::startTextInput() : Could not start text input: " << SDL_GetError() << "\n";
        return false;    
    }
    return true;
}

bool zmain::startTextInput(const inputProperties& props) {
    if (!SDL_StartTextInputWithProperties(window, props)) {
        std::cerr << "Error: zmain::startTextInput() Could not start text input with properties: " << SDL_GetError() << "\n";
        return false;    
    }
    return true;
}

bool zmain::stopTextInput() {
    if (!SDL_StopTextInput(window)) {
        std::cerr << "Error: zmain::stopTextInput() Could not stop text input: " << SDL_GetError() << "\n";
        return false;    
    }
    return true;
}

zmain::~zmain() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}