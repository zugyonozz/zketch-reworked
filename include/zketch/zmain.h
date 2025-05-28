// zmain.h
#pragma once
#include "ztype.h"

class zmain {
private:
    Window window;
    Renderer renderer;

    bool init();
    bool createWindow(const char* title, int W, int H, windowFlags flags);
    bool createRenderer(Window window, const char* driver = nullptr);

public:
    zmain();
    zmain(const char* title, int W, int H, windowFlags flags);
    zmain(const char* title, int W, int H, windowFlags flags, const char* driver);

    const FPoint getWinSize() const;
    Renderer& getRenderer();
    
    bool present();
    bool clearRender(const Color& color);
    void delay(Uint32 ms);
	bool startTextInput();
    bool startTextInput(const inputProperties& props);
    bool stopTextInput();

    bool setWindowIcon(const char* path);
	bool setWindowMinSize(const int& W, const int& H);
    
    ~zmain();
};