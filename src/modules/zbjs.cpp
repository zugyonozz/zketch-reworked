// zbjs.cpp
#include "ztype.h"
#include "zbj.h"
#include "zprop.h"
#include <memory>
#include <vector>
#include "zbjs.h"
#include "utils.h"
#include <iostream>

void zbjs::handleLine(Renderer& renderer){
	auto temp = std::make_unique<zbj>();
	FPoint p1, p2; std::string c;
	if(!(props->getProp("xy1", p1) 
		&& props->getProp("xy2", p2) 
		&& props->getProp("Color", c))){
			std::cerr << "Error -> Could not parse LINE object. Make sure you have \n-\"xy1\", \n-\"xy2\", & \n-\"Color\" properties.\n";
			return;
	}
	temp->setColor(Hex(c.c_str()));
	temp->draw(renderer, p1, p2);
	items.emplace_back(std::move(temp));
}

void zbjs::handleRect(Renderer& renderer){
	auto temp = std::make_unique<zbj>();
	FBound b; float r, w; std::string c, cb;
	if(!(props->getProp("Boundary", b) 
	&& props->getProp("Background", c) 
	&& props->getProp("Roundness", r)
	&& props->getProp("Border", w)
	&& props->getProp("Border-Color", cb))){
		std::cerr << "Error -> Could not parse RECT object. Make sure you have \n-\"Boundary\", \n-\"Roundness\", \"Border\", \"Border-Color\", & \n-\"Background\" properties.\n";
		return;
	}
	if(w < 0.0f){ std::cerr << "Error -> Invalid border thickness value!\n"; return; }
	if(w != 0.0f && w > 0.0f){
		auto temp2 = std::make_unique<zbj>();
		temp2->setColor(Hex(cb.c_str()));
		temp2->setTransform(b);
		temp2->getTransform().setScale((b.w + w * 2) / b.w, (b.h + w * 2) / b.h);
		temp2->draw(renderer, r);
		items.emplace_back(std::move(temp2));
	}
	temp->setColor(Hex(c.c_str()));
	temp->setTransform(b);
	temp->draw(renderer, r);
	items.emplace_back(std::move(temp));
}

void zbjs::handleText(Renderer& renderer){
	auto temp = std::make_unique<zbj>();
	FPoint p; std::string c, t; Font f;
	if(!(props->getProp("Position", p) 
	&& props->getProp("Text", t) 
	&& props->getProp("Color", c) 
	&& props->getProp("Font", f))){
		std::cerr << "Error -> Could not parse TEXT object. Make sure you have: \n-\"Position\", \n-\"Text\", \n-\"Font\", & \n-\"Color\" properties.\n";
		return;
	}
	temp->setColor(Hex(c.c_str()));
	temp->draw(renderer, f, t.c_str(), p);
	items.emplace_back(std::move(temp));
}

void zbjs::handleImage(Renderer& renderer){
	auto temp = std::make_unique<zbj>();
	FPoint po; std::string pa; float s;
	if(!(props->getProp("Path", pa) 
	&& props->getProp("Position", po) 
	&& props->getProp("Scale", s))){
		std::cerr << "Error -> Could not parse IMAGE object. Make sure you have \n-\"Path\", \n-\"Position\", & \n-\"Scale\" properties.\n";
		return;
	}
	temp->draw(renderer, pa.c_str());
	temp->setTransform(po);
	temp->getTransform().setScale(s);
	items.emplace_back(std::move(temp));
}

void zbjs::handlePolygon(Renderer& renderer){
	auto temp = std::make_unique<zbj>();
	FPoint p; std::vector<int> i; std::vector<Vertex> v; std::string c;
	if(!(props->getProp("Vertices", v) 
	&& props->getProp("Position", p) 
	&& props->getProp("Indices", i)
	&& props->getProp("Color", c))){
		std::cerr << "Error -> Could not parse POLYGON object. Make sure you have \n-\"Vertices\", \n-\"Position\", \n-\"Color\", & \n-\"Indices\" properties.\n";
		return;
	}
	temp->draw(renderer, i, v);
	temp->setColor(Hex(c.c_str()));
	temp->setTransform(p);
	items.emplace_back(std::move(temp));
}

void zbjs::handleTextBox(Renderer& renderer){
	handleRect(renderer);
	// TEXT
	handleText(renderer);
}

zbjs::zbjs(Renderer& renderer, std::unique_ptr<objProp> props) : props(std::move(props)) {
	switch (this->props->getType()){
		case objType::LINE : handleLine(renderer); break;
		case objType::RECT : handleRect(renderer); break;
		case objType::TEXT : handleText(renderer); break;
		case objType::IMAGE: handleImage(renderer); break;
		case objType::POLYGON: handlePolygon(renderer); break;
		case objType::TEXTBOX: handleTextBox(renderer); break;
		default:
			std::cerr << "Error -> invalid object type. Make sure you have a valid type.\n";
			break;
		}
}

zbjs::~zbjs(){
	if(!items.empty()){
		for(auto& i : items){ i->reset(); }
	}
}

void zbjs::addItem(Renderer& renderer, zbj&& item){
	items.emplace_back(std::make_unique<zbj>(std::move(item)));
}

void zbjs::insertItem(Renderer& renderer, std::unique_ptr<zbj> item, size_t id) {
	if (id >= items.size()) {
		items.emplace_back(std::move(item));
	} else {
		items.insert(items.begin() + id, std::move(item));
	}
}

void zbjs::show(Renderer& renderer){
	if(items.empty()){
		std::cerr << "Error -> Could not show objects, items empty.\n";
		return;
	}
	for(auto& i : items){
		if(!i){
			std::cerr << "Error -> Could not show objects, item null.\n";
			return;
		}
		i->show(renderer);
	}
}

void zbjs::showExclude(Renderer& renderer, size_t id){
	if(items.empty()){
		std::cerr << "Error -> Could not show objects, items empty.\n";
		return;
	}
	for(size_t i = 0; i < items.size() ; i++){
		if(!items[i]){
			std::cerr << "Error -> Could not show objects, item null.\n";
			return;
		}
		if(i != id){
			items[i]->show(renderer);
		}
	}
}

void zbjs::showExclude(Renderer& renderer, std::vector<size_t> id){
	if(items.empty()){
		std::cerr << "Error -> Could not show objects, items empty.\n";
		return;
	}
	for(size_t i = 0; i < items.size() ; i++){
		bool show = true;
		if(!items[i]){
			std::cerr << "Error -> Could not show objects, item null.\n";
			return;
		}
		for(size_t j = 0; j < id.size(); j++){
			if(i == id[j]){
				show = false;
				break;
			}
		}
		if(show){ items[i]->show(renderer); }
	}
}

void zbjs::removeItem(size_t id) {
	if (id >= items.size()) {
		std::cerr << "Error -> invalid id. Make sure you have a valid id.\n";
		return;
	}
	if (items[id]) items[id]->reset();
	items.erase(items.begin() + id); // Otomatis delete, karena unique_ptr
}


const objProp& zbjs::getProps() const{
	return *props;
}

const std::vector<std::unique_ptr<zbj>>& zbjs::getItems() const {
	return items;
}