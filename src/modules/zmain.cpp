#include "zmain.h"

zmain::zmain() : window(nullptr), renderer(nullptr) {}

zmain::zmain(const std::string& title, int& W, int& H, windowFlags flags) {
	createWindow(title.c_str(), W, H, flags);
	createRenderer(window);
	init();
}

zmain::zmain(const std::string& title, int& W, int& H, windowFlags flags, const char* driver){
	createWindow(title.c_str(), W, H, flags);
	createRenderer(window, driver);
	init();
}

void zmain::createWindow(const char* title, int& W, int& H, windowFlags flags){
	window = SDL_CreateWindow(title, W, H, flags);
}

void zmain::createRenderer(Window window, const char* driver = nullptr){
	renderer = SDL_CreateRenderer(window, nullptr);
}

void zmain::init() {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "Error -> Could not initialize : " << SDL_GetError() << "\n";
		return;
	}
	if (!TTF_Init()) {
		std::cerr << "Error -> Could not initialize TTF : " << SDL_GetError() << "\n";
		return;
	}
	if (!window) {
		std::cerr << "Error -> Could not create window: " << SDL_GetError() << "\n";
		return;
	}
	if (!renderer) {
		std::cerr << "Error -> Could not create renderer: " << SDL_GetError() << "\n";
		return;
	}
}

bool zmain::present(){
	return SDL_RenderPresent(renderer);
}

Point zmain::getWinSize() const {
	Point p;
	if(!SDL_GetWindowSize(window, &p.x, &p.y)){
		std::cerr << "Error -> Could not get window size: " << SDL_GetError() << "\n";
		return {};
	}
	return p;
}

Renderer& zmain::getRenderer() {
	return renderer;
}

bool zmain::clearRender(Color color){
	if(!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)){
		std::cerr << "Error -> Could not set color : " << SDL_GetError() << "\n";
		return false;
	}
	if(!SDL_RenderClear(renderer)){
		std::cerr << "Error -> Could not render clear! : " << SDL_GetError() << "\n";
		return false;
	}
	return true;
}

void zmain::delay(Uint32 ms){
	SDL_Delay(ms);
}

void zmain::setWindowIcon(const char* path){
	SDL_Surface* icon = IMG_Load(path);
	if (icon) {
		SDL_SetWindowIcon(window, icon);
		SDL_DestroySurface(icon);
	} else {
		std::cerr << "Error: Could not load icon!: " << SDL_GetError() << "\n";
	}
}

bool zmain::startTextInput(){
	if(!SDL_StartTextInput(window)){
		std::cerr << "Error: Could not start text input: " << SDL_GetError() << "\n";
		return false;	
	}
	return true;
}

bool zmain::startTextInput(inputProperties props){
	if(!SDL_StartTextInputWithProperties(window, props)){
		std::cerr << "Error: Could not start text input: " << SDL_GetError() << "\n";
		return false;	
	}
	return true;
}

bool zmain::stopTextInput(){
	if(!SDL_StopTextInput(window)){
		std::cerr << "Error: Could not stop text input: " << SDL_GetError() << "\n";
		return;	
	}
}

zmain::~zmain() {
	if (renderer) { SDL_DestroyRenderer(renderer); }
	if (window) { SDL_DestroyWindow(window); }
	TTF_Quit();
	SDL_Quit();
}