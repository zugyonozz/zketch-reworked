// zmain.h
#pragma once
#include "zenv.h"

using windowFlags = SDL_WindowFlags;
using inputProperties = SDL_PropertiesID;

class zmain {
private:
    Window window;
    Renderer renderer;

    bool init();
    bool createWindow(const char* title, int W, int H, windowFlags flags);
    bool createRenderer(Window window, const char* driver = nullptr);

public:
    zmain();
    zmain(const std::string& title, int W, int H, windowFlags flags);
    zmain(const std::string& title, int W, int H, windowFlags flags, const char* driver);

    const FPoint getWinSize() const;
    Renderer& getRenderer();
    
    bool present();
    bool clearRender(const Color& color);
    void delay(Uint32 ms);
    bool setWindowIcon(const char* path);
    bool startTextInput();
    bool startTextInput(const inputProperties& props);
    bool stopTextInput();
    
    ~zmain();
};