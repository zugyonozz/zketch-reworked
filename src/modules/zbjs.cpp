#include "zbjs.h"

void zbjs::handleLine(Renderer& renderer, std::unique_ptr<zbj>& item){
	FPoint p1, p2; std::string c;
	if(!(props.getProp("xy1", p1) 
		&& props.getProp("xy2", p2) 
		&& props.getProp("Color", c))){
			std::cerr << "Error -> Could not parse LINE object. Make sure you have \n-\"xy1\", \n-\"xy2\", & \n-\"Color\" properties.\n";
			return;
	}
	item->setColor(Hex(c.c_str()));
	item->draw(renderer, p1, p2);
	items.emplace_back(std::move(item));
}

void zbjs::handleRect(Renderer& renderer, std::unique_ptr<zbj>& item){
	FBound b; float r, w; std::string c, cb;
	if(!(props.getProp("Boundary", b) 
	&& props.getProp("Background", c) 
	&& props.getProp("Roundness", r)
	&& props.getProp("Border", w)
	&& props.getProp("Border-Color", cb))){
		std::cerr << "Error -> Could not parse RECT object. Make sure you have \n-\"Boundary\", \n-\"Roundness\", \"Border\", \"Border-Color\", & \n-\"Background\" properties.\n";
		return;
	}
	if(w != 0.0f && w > 0.0f){
		auto tmp2 = std::make_unique<zbj>();
		tmp2->setColor(Hex(cb.c_str()));
		tmp2->setTransform(b);
		tmp2->getTransform().setScale((b.w + w * 2) / b.w, (b.h + w * 2) / b.h);
		tmp2->draw(renderer, r);
		items.emplace_back(std::move(tmp2));
	}
	if(w < 0.0f){
		std::cerr << "Error -> Invalid border thickness value!\n";
		return;
	}
	item->setColor(Hex(c.c_str()));
	item->setTransform(b);
	items.emplace_back(std::move(item));
}

void zbjs::handleText(Renderer& renderer, std::unique_ptr<zbj>& item){
	FPoint p; std::string c, t; Font f;
	if(!(props.getProp("Position", p) 
	&& props.getProp("Text", t) 
	&& props.getProp("Color", c) 
	&& props.getProp("Font", f))){
		std::cerr << "Error -> Could not parse TEXT object. Make sure you have: \n-\"Position\", \n-\"Text\", \n-\"Font\", & \n-\"Color\" properties.\n";
		return;
	}
	item->setColor(Hex(c.c_str()));
	item->draw(renderer, f, t.c_str(), p);
	items.emplace_back(std::move(item));
}

void zbjs::handleImage(Renderer& renderer, std::unique_ptr<zbj>& item){
	FPoint po; std::string pa; float s;
	if(!(props.getProp("Path", pa) 
	&& props.getProp("Position", po) 
	&& props.getProp("Scale", s))){
		std::cerr << "Error -> Could not parse IMAGE object. Make sure you have \n-\"Path\", \n-\"Position\", & \n-\"Scale\" properties.\n";
		return;
	}
	item->draw(renderer, pa.c_str());
	item->setTransform(po);
	item->getTransform().setScale(s);
	items.emplace_back(std::move(item));
}

void zbjs::handlePolygon(Renderer& renderer, std::unique_ptr<zbj>& item){
	FPoint p; std::vector<int> i; std::vector<Vertex> v; std::string c;
	if(!(props.getProp("Vertices", v) 
	&& props.getProp("Position", p) 
	&& props.getProp("Indices", i)
	&& props.getProp("Color", c))){
		std::cerr << "Error -> Could not parse POLYGON object. Make sure you have \n-\"Vertices\", \n-\"Position\", \n-\"Color\", & \n-\"Indices\" properties.\n";
		return;
	}
	item->draw(renderer, i, v);
	item->setColor(Hex(c.c_str()));
	item->setTransform(p);
	items.emplace_back(std::move(item));
}

void zbjs::handleTextBox(Renderer& renderer){
	auto item = std::make_unique<zbj>();
	handleRect(renderer, item);
	// TEXT
	auto tx = std::make_unique<zbj>();
	handleText(renderer, tx);
}

zbjs::zbjs(Renderer& renderer, objProp& props) : props(props) {
	auto temp = std::make_unique<zbj>();
	switch (props.type){
		case objType::LINE : handleLine(renderer, temp); break;
		case objType::RECT : handleRect(renderer, temp); break;
		case objType::TEXT : handleText(renderer, temp); break;
		case objType::IMAGE: handleImage(renderer, temp); break;
		case objType::POLYGON: handlePolygon(renderer, temp); break;
		case objType::TEXTBOX: handleTextBox(renderer); break;
		default:
			std::cerr << "Error -> invalid object type. Make sure you have a valid type.\n";
			break;
		}
}

zbjs::~zbjs(){
	if(!items.empty()){
		for(auto& i : items){
			i->clear();
		}
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
		for(int j = 0; j < id.size(); j++){
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
	if (items[id]) {
		items[id]->clear(); // Optional, kalau kamu punya logic cleanup sendiri
	}
	items.erase(items.begin() + id); // Otomatis delete, karena unique_ptr
}


const objProp& zbjs::getProps() const{
	return props;
}

const std::vector<std::unique_ptr<zbj>>& zbjs::getItems() const {
	return items;
}