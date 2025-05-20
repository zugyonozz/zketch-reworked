#include "zketch.h"
using namespace std;

zmain app("mySAQ", 1200, 900, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
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

	objProp textProp = {objType::RECT};
	textProp.props["Boundary"] = FBound({200.0f, 200.0f, 100.0f, 100.0f});
	textProp.props["Color"] = "FFF";
	textProp.props["Roundness"] = 1.0f;

	zbjs textObj(app.getRenderer(), textProp);

	zbj obj = {Transform({0, 0, 100, 100}), Hex("#0000F0")};
	obj.setAnchor(Anchor::ANCHOR_TOP_MID);
	obj.draw(app.getRenderer());

	zbj text = {Transform({100, 100, 0, 0}), Hex("FF00FF")};
	text.draw(app.getRenderer(), fonts.getFont("Montserrat-Medium", 28), "Hello World", {100, 100});

	while(isRun){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){
				isRun = false;
			}
		}
		app.clearRender(Hex("000"));
		textObj.show(app.getRenderer());
		// text.show(app.getRenderer());
		// obj.show(app.getRenderer());
		if(!app.present()){
			std::cerr << "Error -> zmain::present couldn't show rendered\n";
		}
		app.delay(16);
	}

	return 0;
}