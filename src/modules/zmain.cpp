#include "zmain.h"

zmain::zmain() : window(nullptr), renderer(nullptr) {}

zmain::zmain(const std::string& title, int W, int H, windowFlags flags) {
    if (createWindow(title.c_str(), W, H, flags)) {
        if (createRenderer(window)) {
            init();
        }
    }
}

zmain::zmain(const std::string& title, int W, int H, windowFlags flags, const char* driver) {
    if (createWindow(title.c_str(), W, H, flags)) {
        if (createRenderer(window, driver)) {
            init();
        }
    }
}

bool zmain::createWindow(const char* title, int W, int H, windowFlags flags) {
    window = SDL_CreateWindow(title, W, H, flags);
    if (!window) {
        std::cerr << "Error -> Could not create window: " << SDL_GetError() << "\n";
        return false;
    }
    return true;
}

bool zmain::createRenderer(Window window, const char* driver) {
    renderer = SDL_CreateRenderer(window, driver);
    if (!renderer) {
        std::cerr << "Error -> Could not create renderer: " << SDL_GetError() << "\n";
        return false;
    }
    return true;
}

bool zmain::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error -> Could not initialize SDL: " << SDL_GetError() << "\n";
        return false;
    }
    
    if (TTF_Init() != 0) {
        std::cerr << "Error -> Could not initialize TTF: " << SDL_GetError() << "\n";
        SDL_Quit();
        return false;
    }
    
    return true;
}

bool zmain::present() {
    return SDL_RenderPresent(renderer) == 0;
}

Point zmain::getWinSize() const {
    Point p{0, 0};
    if (window) {
        SDL_GetWindowSize(window, &p.x, &p.y);
    }
    return p;
}

Renderer& zmain::getRenderer() {
    return renderer;
}

bool zmain::clearRender(const Color& color) {
    if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) != 0) {
        std::cerr << "Error -> Could not set color: " << SDL_GetError() << "\n";
        return false;
    }
    
    if (SDL_RenderClear(renderer) != 0) {
        std::cerr << "Error -> Could not render clear: " << SDL_GetError() << "\n";
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
        std::cerr << "Error: Could not load icon: " << SDL_GetError() << "\n";
        return false;
    }
    
    SDL_SetWindowIcon(window, icon);
    SDL_DestroySurface(icon);
    return true;
}

bool zmain::startTextInput() {
    if (SDL_StartTextInput(window) != 0) {
        std::cerr << "Error: Could not start text input: " << SDL_GetError() << "\n";
        return false;    
    }
    return true;
}

bool zmain::startTextInput(const inputProperties& props) {
    if (SDL_StartTextInputWithProperties(window, props) != 0) {
        std::cerr << "Error: Could not start text input with properties: " << SDL_GetError() << "\n";
        return false;    
    }
    return true;
}

bool zmain::stopTextInput() {
    if (SDL_StopTextInput(window) != 0) {
        std::cerr << "Error: Could not stop text input: " << SDL_GetError() << "\n";
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
    TTF_Quit();
    SDL_Quit();
}