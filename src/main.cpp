#include "zketch.h"
using namespace std;

zmain app = {"mySAQ", 1200, 900, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED};
zfont fonts;
Event e;
bool isRun = true;

void loadFonts(){
	string parent = "res/fonts/Montserrats/";
	vector<pair<string, pair<string, int>>> myfont = {
		{"Montserrat-SemiBold", {"Montserrat-SemiBold.ttf", 36}}, 
		{"Montserrat-Medium", {"Montserrat-Medium.ttf", 24}},
		{"Montserrat-Medium", {"Montserrat-Medium.ttf", 28}}, 
		{"Montserrat-Light", {"Montserrat-Light.ttf", 20}}
	};
	for(auto& f : myfont){
		string fullPath = parent + f.second.first;
		fonts.loadFont(f.first, f.second.second, fullPath.c_str());
	}
}

int main(){
	loadFonts();

	objProp textProp = {objType::TEXT};
	textProp.props["Text"] = "Hello World!";
	textProp.props["Font"] = fonts.getFont("Montserrat-Medium", 24);
	textProp.props["Color"] = "000000";
	textProp.props["Position"] = FPoint{0.0f, 0.0f};

	zbjs textObj(app.getRenderer(), textProp);

	while(isRun){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){
				isRun = false;
			}
		}
		app.clearRender(Hex("000"));
		textObj.show(app.getRenderer());
		app.present();
		app.delay(16);
	}

	app.stopTextInput();
	return 0;
}