#include "zketch.h"
using namespace std;

zmain app("zketch demo", 1200, 900, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
zfont fonts;
Event e;
bool isRun = true;
float W = app.getWinSize().x;
float H = app.getWinSize().y;

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

void UpdatePH(zbj& z, string& text, bool isPw = false){
	if(!z.getTexture()){
		cerr << "Error -> UpdatePH : zbj no texture!\n";
	}
	z.clear();
	z.setColor(Hex("303030"));
	z.setAnchor(Anchor::ANCHOR_TOP_LEFT);

	string display;
	if(isPw){
		display = string(text.size(), '*');
		z.draw(app.getRenderer(), fonts.getFont("Montserrat-Light", 20), display.c_str(), {W*0.63f, H*0.5f+36});
		return;
	}else{
		display = text;
		z.draw(app.getRenderer(), fonts.getFont("Montserrat-Light", 20), display.c_str(), {W*0.63f, H*0.4f+36});
	}
}

void updatePHcursor(zbj& z, vector<FPoint> p, size_t id){
	switch(id){
		case 0 : return;
		case 1 : z.setTransform(p[0]); break;
		case 2 : z.setTransform(p[1]); break;
	}
	z.show(app.getRenderer());
}

void updateSubmit(zbjs& z, bool state){
	if(state){
		z.showExclude(app.getRenderer(), 0);
		return;
	}else{
		z.showExclude(app.getRenderer(), 1);
		return;
	}
}

int main(){
	loadFonts();

	zbj background = {Transform({0.0f, 0.0f, W, H}, Anchor::ANCHOR_TOP_LEFT), Hex("6497b1")};
	background.draw(app.getRenderer());

	zbj sideBar = {Transform({W, 0.0f, W*0.4f, H}, Anchor::ANCHOR_TOP_RIGHT), Hex("FAFAFA")};
	sideBar.draw(app.getRenderer());

	zbj h1a;
	h1a.setColor(Hex("6497b1"));
	h1a.setAnchor(Anchor::ANCHOR_TOP_LEFT);
	h1a.draw(app.getRenderer(), fonts.getFont("Montserrat-SemiBold", 36), "Wellcome to Zketch!", {W*0.63f, H*0.3f});

	zbj h2a;
	h2a.setColor(Hex("303030"));
	h2a.setAnchor(Anchor::ANCHOR_TOP_LEFT);
	h2a.draw(app.getRenderer(), fonts.getFont("Montserrat-Medium", 24), "Please fill in the form below!", {W*0.63f, H*0.3f+54});

	zbj h3a;
	h3a.setColor(Hex("303030"));
	h3a.setAnchor(Anchor::ANCHOR_TOP_LEFT);
	h3a.draw(app.getRenderer(), fonts.getFont("Montserrat-Light", 20), "Username", {W*0.63f, H*0.4f});

	string UNstr = "";
	zbj UNtext;
	UNtext.setColor(Hex("303030"));
	UNtext.setAnchor(Anchor::ANCHOR_TOP_LEFT);
	UNtext.draw(app.getRenderer(), fonts.getFont("Montserrat-Light", 20), UNstr.c_str(), {W*0.63f, H*0.4f+36});

	zbj line1 = {Transform({W*0.63f, H*0.4f+72, W*0.34f, H*0.001f}, Anchor::ANCHOR_TOP_LEFT), Hex("303030")};
	line1.draw(app.getRenderer());

	zbj h3b;
	h3b.setColor(Hex("303030"));
	h3b.setAnchor(Anchor::ANCHOR_TOP_LEFT);
	h3b.draw(app.getRenderer(), fonts.getFont("Montserrat-Light", 20), "Password", {W*0.63f, H*0.5f});

	string PWstr = "";
	zbj PWtext;
	PWtext.setColor(Hex("303030"));
	PWtext.setAnchor(Anchor::ANCHOR_TOP_LEFT);
	PWtext.draw(app.getRenderer(), fonts.getFont("Montserrat-Light", 20), PWstr.c_str(), {W*0.63f, H*0.5f+36});

	zbj line2 = {Transform({W*0.63f, H*0.5f+72, W*0.34f, H*0.001f}, Anchor::ANCHOR_TOP_LEFT), Hex("303030")};
	line2.draw(app.getRenderer());

	zbj h2b;
	h2b.setColor(Hex("6497b1"));
	h2b.setAnchor(Anchor::ANCHOR_TOP_RIGHT);
	h2b.draw(app.getRenderer(), fonts.getFont("Montserrat-Medium", 24), "forget password?", {W*0.97f, H*0.6f-24});

	objProp submitProp(objType::TEXTBOX);
	submitProp.props["Boundary"] = FBound{W*0.8f, H*0.7f, W*0.15f, 72.0f};
	submitProp.props["Background"] = "6497b1";
	submitProp.props["Roundness"] = 0.28f;
	submitProp.props["Text"] = "Login";
	submitProp.props["Font"] = fonts.getFont("Montserrat-Medium", 28);
	submitProp.props["Position"] = FPoint{W*0.8f, H*0.7f};
	submitProp.props["Color"] = "FAFAFA";
	zbjs submit(app.getRenderer(), submitProp);
	auto submit2 = make_unique<zbj>(Transform(submit.getItems()[0]->getTransform().getBound(), Anchor::ANCHOR_TOP_LEFT), Hex("107dac"));
	submit2->draw(app.getRenderer(), 0.28);
	submit.insertItem(app.getRenderer(), move(submit2), 1);
	FBound submitTrigger = submit.getItems()[0]->getTransform().getBound();


	size_t clickId = 0;
	FBound UNtrigger = { 
		line1.getTransform().getBound().x, h3a.getTransform().getBound().y + 
		h3a.getTransform().getBound().h, line1.getTransform().getBound().w, 48};

	FBound PWtrigger = { 
		line2.getTransform().getBound().x, h3b.getTransform().getBound().y + 
		h3b.getTransform().getBound().h, line2.getTransform().getBound().w, 48};

	zbj PHcursor = {Transform({0.0f, 0.0f, W*0.001f, 20.0f}, Anchor::ANCHOR_TOP_LEFT), Hex("6497b1")};
	PHcursor.draw(app.getRenderer());
	vector<FPoint> cursorPos;

	zFuncs callshow = {
		[&](){ background.show(app.getRenderer()); },
		[&](){ sideBar.show(app.getRenderer()); },
		[&](){ h1a.show(app.getRenderer()); },
		[&](){ h2a.show(app.getRenderer()); },
		[&](){ h3a.show(app.getRenderer()); },
		[&](){ line1.show(app.getRenderer()); },
		[&](){ h3b.show(app.getRenderer()); },
		[&](){ line2.show(app.getRenderer()); },
		[&](){ h2b.show(app.getRenderer()); },
		[&](){ updateSubmit(submit, isHover(FBound2Bound(submitTrigger))); },
		[&](){ UNtext.show(app.getRenderer()); },
		[&](){ PWtext.show(app.getRenderer()); },
		[&](){ updatePHcursor(PHcursor, cursorPos, clickId); }
	};

	app.startTextInput();

	while(isRun){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){ isRun = false; }
			if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
				if(isOnClick(e, FBound2Bound(UNtrigger)) && clickId != 1){
					clickId = 1;
				}else if(isOnClick(e, FBound2Bound(PWtrigger)) && clickId != 2){
					clickId = 2;
				}else{
					clickId = 0;
				}
			}
			if(e.type == SDL_EVENT_TEXT_INPUT){
				if(clickId == 1){
					UNstr += e.text.text;
					UpdatePH(UNtext, UNstr);
					break;
				}else if(clickId == 2){
					PWstr += e.text.text;
					UpdatePH(PWtext, PWstr, 1);
					break;
				}
			}
		}
		cursorPos = {
			{UNtrigger.x + UNtext.getTransform().getBound().w, UNtrigger.y + 12}, 
			{PWtrigger.x + PWtext.getTransform().getBound().w, PWtrigger.y + 12}
		};
		app.clearRender(Hex("000"));
		callshow();
		app.present();
		app.delay(16);
	}

	app.stopTextInput();
	return 0;
}