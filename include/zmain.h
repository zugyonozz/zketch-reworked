// zmain.h
#pragma once
#include "zenv.h"

class zmain {
private:
    Window window;
    Renderer renderer;

	void init();
	void createWindow(const char* title, int& W, int& H, windowFlags flags);
	void createRenderer(Window window, const char* driver = nullptr);

public:
    zmain();
    zmain(const std::string& title, int& W, int& H, windowFlags flags);
	zmain(const std::string& title, int& W, int& H, windowFlags flags, const char* driver);

    Point getWinSize() const ;
    Renderer& getRenderer();
	
	bool present();
	bool clearRender(Color color);
	void delay(Uint32 ms);
	void setWindowIcon(const char* path);
	bool startTextInput();
	bool startTextInput(inputProperties props);
	bool stopTextInput();
	
    ~zmain();
};