#include "zketch.h"
using namespace std;

WinData wd = {"mySAQ", 1200, 900, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED};

zmain app(wd);

zfont fonts;
Event e;
bool isRun = true;

Bound bBg;
Bound bLayout;
vector<Point> pTitle;
vector<Point> pTextinForm;
vector<pair<string, pair<string, int>>> dTextinForm;

struct inputField{
	Bound b;
	bool isFocused;
	string t;
	bool isPassword;
};

vector<inputField> inputFields;
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

void setBg(zbj& z){
	bBg = {0, 0, app.getWinSize().w, app.getWinSize().h};
	z.setBound(bBg);
	z.setColor(Hex("7070FA"));
	z.draw();
}

void setLayout(zbj& z){
	bLayout = {app.getWinSize().w, 0, app.getWinSize().w / 3, app.getWinSize().h};
	z.setBound(bLayout);
	if(!z.getTextures().front()){
		z.setColor(Hex("303030"));
		z.setAnchorPt(AnchorType::ANCHOR_TOP_RIGHT);
		z.draw();
	}
}

void setTextinForm(zbj& z){
	pTextinForm = {
		{(int)(app.getWinSize().w * 0.7), app.getWinSize().h / 3},
		{(int)(app.getWinSize().w * 0.7), app.getWinSize().h / 3 + 50}
	};

	dTextinForm = {
		{"Welcome to mySAQ!", {"Montserrat-SemiBold", 36}}, 
		{"Enter your account details below.", {"Montserrat-Medium", 24}},
	};

	vector<Point>& p = pTextinForm;
	vector<pair<string, pair<string, int>>>& d = dTextinForm;

	z.setColor(Hex("#F1F1F1"));
	if(!z.getTextures().front()){
		for(size_t i = 0; i < d.size(); i++){
			z.draw(fonts.getFont(d[i].second.first, d[i].second.second), d[i].first.c_str(), pTextinForm[i]);
			if(i < d.size() - 1){ z.addItem(); }
		}
	}else{
		for(size_t i = 0; i < z.getBounds().size(); i++){
			z.setActiveID(i);
			Bound b = z.getBounds()[i].origin;
			b.x = p[i].x;
			b.y = p[i].y;
			z.setBound(b);
		}
	}
}

void setTitlePH(zbj& z){
	pTitle = {
		{(int)(app.getWinSize().w * 0.7), app.getWinSize().h / 3 + 100},
		{(int)(app.getWinSize().w * 0.7), app.getWinSize().h / 3 + 200}
	};
	vector<string> s = {"Username", "Password"};

	z.setColor(Hex("f1f1f1"));
	for(size_t i = 0; i < s.size(); i++){
		z.draw(fonts.getFont("Montserrat-Light", 20), s.at(i).c_str(), pTitle.at(i));
		if(i < s.size() - 1){ z.addItem(); }
	}
}

void setPH(zbj& z, const inputField& f){
	Color c = Hex("707070");

	if(!z.getTextures().front()){
		z.setColor(c);
		z.setBound(f.b);
		z.draw(0.5);
	}
	if(z.getBounds().size() == 2){
		z.removeItem(1);
	}
	z.addItem();
	z.setColor(Hex("f1f1f1"));

	string display = f.t;
	if(f.isPassword && !display.empty()){
		display = string(display.size(), '*');
	}
	if(f.isFocused){
		display += '|';
	}
	z.draw(fonts.getFont("Montserrat-Light", 20), display.c_str(), {f.b.x + 15, f.b.y + int(f.b.h * 0.30)});
}

void setSubmitBtn(zbj& z){
	z.setBound({(int)(app.getWinSize().w * 0.74), app.getWinSize().h / 3 + 330, (int)(app.getWinSize().w * 0.2), 75});
	z.setColor(Hex("7070FA"));
	z.draw(0.25);
	z.addItem();
	z.setColor(Hex("9090FA"));
	z.draw(0.25);
	z.addItem();
	z.setColor(Hex("f1f1f1"));
	z.draw(fonts.getFont("Montserrat-Medium", 28), "Log in", {(int)(app.getWinSize().w * 0.81), app.getWinSize().h / 3 + 350});
}

void showBtnOnClick(zbj& z, bool isClick){
	if(isClick){
		for(size_t i = 1; i < z.getBounds().size(); i++){
			z.show(i);
		}
		return;
	}
	z.show();
};

int main(){
	loadFonts();
	app.updateWinSize();
	app.startTextInput();

	zbj bg(app.getRenderer());
	setBg(bg);

	zbj layout(app.getRenderer());
	setLayout(layout);

	zbj textinForm(app.getRenderer());
	setTextinForm(textinForm);

	zbj usnm(app.getRenderer());
	setTitlePH(usnm);

	Bound busnm = {(int)(app.getWinSize().w * 0.7), app.getWinSize().h / 3 + 130, (int)(app.getWinSize().w * 0.275), 50};
	Bound bpass = {(int)(app.getWinSize().w * 0.7), app.getWinSize().h / 3 + 230, (int)(app.getWinSize().w * 0.275), 50};
	inputFields = {{busnm, false, "", false}, {bpass, false, "", false}};

	zbj phusnm(app.getRenderer());
	setPH(phusnm, inputFields[0]);

	zbj phpass(app.getRenderer());
	setPH(phpass, inputFields[1]);

	zbj submitBtn(app.getRenderer());
	setSubmitBtn(submitBtn);

	zFuncs callShowObj = {
		[&](){return bg.show();}, 
		[&](){return layout.show();}, 
		[&](){return textinForm.show();}, 
		[&](){return usnm.show();}, 
		[&](){return phusnm.show();}, 
		[&](){return phpass.show();}
		// [&](){return submitBtn.show();},
	};

	while(isRun){
		bool isSubmitClick;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){
				isRun = false;
			}
			if(e.type == SDL_EVENT_WINDOW_MAXIMIZED || e.type == SDL_EVENT_WINDOW_MINIMIZED || e.type == SDL_EVENT_WINDOW_RESTORED 
			|| (e.type == SDL_EVENT_WINDOW_RESIZED && app.getWinSize().w >= 1200 && app.getWinSize().h >= 900)){
				app.updateWinSize();
			}
			if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
				for(size_t i = 0; i < inputFields.size(); i++){
					if(isOnClick(e, inputFields[i].b)){
						inputFields[i].isFocused = true;
						break;
					}else{
						inputFields[i].isFocused = false;
					}
					isSubmitClick = isOnClick(e, submitBtn.getBounds()[0].origin);
				}
			}
			if(e.type == SDL_EVENT_TEXT_INPUT){
				for(auto& i : inputFields){
					if(i.isFocused){
						i.t += e.text.text;

						if(&i != &inputFields[0]){
							setPH(phpass, i);
						}else{
							setPH(phusnm, i);
						}
						break;
					}
				}
			}
			if (e.type == SDL_EVENT_KEY_DOWN){
				if(e.key.key == SDLK_BACKSPACE){
					for(auto& i : inputFields){
						if(i.isFocused){
							if(!i.t.empty()){
								i.t.pop_back();
							}
							if(&i != &inputFields[0]){
								setPH(phpass, i);
							}else{
								setPH(phusnm, i);
							}
							break;
						}
					}
				}
			}
			
		}
		app.clearRender(Hex("000"));
		callShowObj();
		showBtnOnClick(submitBtn, isSubmitClick);
		app.present();
		app.delay(16);
	}

	app.stopTextInput();
	return 0;
}