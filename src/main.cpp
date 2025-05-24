// main.cpp
#include "login.h"
#include "zketch.h"
using namespace std;

zmain app("zketch demo", 1200, 900, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
zfont fonts;

enum appState{
	APP_LOGIN,
	APP_REGISTER,
	APP_DASHBOARD,
	APP_EXIT
};

appState state = APP_LOGIN;

void loadFonts(){
	string parent = "res/fonts/Montserrats/";
	vector<pair<string, pair<string, size_t>>> fontlist = {
		{"Montserrat-SemiBold.ttf", {"Montserrat-SemiBold", 36}},
		{"Montserrat-Medium.ttf", {"Montserrat-Medium", 28}},
		{"Montserrat-Medium.ttf", {"Montserrat-Medium", 24}},
		{"Montserrat-Light.ttf", {"Montserrat-Light", 20}},
	};
	for(auto& i : fontlist){
		string f = parent + i.first;
		fonts.loadFont(i.second.first.c_str(), i.second.second, f.c_str());
	}
}

int main(){
	while(state != APP_EXIT){
		switch (state) {
		case 0:
			state = (appState)login();
			break;
		
		default:
			break;
		}
	}
}