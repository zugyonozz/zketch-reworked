#include "zketch.h"
using namespace std;

zmain app("mySAQ", 1200, 900, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
zfont fonts;
Event e;
bool isRun = true;

int main(){
	fonts.loadFont("Montserrat-SemiBold", 36, "res/fonts/Montserrats/Montserrat-SemiBold.ttf");

	zbj player;

	while(isRun){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){ isRun = false; }
		}
		app.clearRender(Hex("000"));
		app.present();
		app.delay(16);
	}
	return 0;
}