#include "zbjs.h"

zbjs::zbjs(Renderer& renderer, objProp& props) : props(props) {
	auto temp = std::make_unique<zbj>();
	switch (props.type){
		case objType::LINE : {
			FPoint p1, p2;
			std::string c;
			if(!(props.getProp("xy1", p1) 
			&& props.getProp("xy2", p2) 
			&& props.getProp("Color", c))){
				std::cerr << "Error -> Could not parse LINE object. Make sure you have \n-\"xy1\", \n-\"xy2\", & \n-\"Color\" properties.\n";
				break;
			}
			temp->setColor(Hex(c.c_str()));
			temp->draw(renderer, p1, p2);
			items.emplace_back(std::move(temp));
			break;
		}
		case objType::RECT : {
			FBound b;
			float r;
			std::string c;
			if(!(props.getProp("Boundary", b) 
			&& props.getProp("Color", c) 
			&& props.getProp("Roundness", r))){
				std::cerr << "Error -> Could not parse RECT object. Make sure you have \n-\"Boundary\", \n-\"Roundness\", & \n-\"Color\" properties.\n";
				break;
			}
			temp->setColor(Hex(c.c_str()));
			temp->setTransform(b);
			temp->draw(renderer, r);
			items.emplace_back(std::move(temp));
			break;
		}
		case objType::TEXT : {
			FPoint p;
			std::string c, t;
			Font f;
			if(!(props.getProp("Position", p) 
			&& props.getProp("Text", t) 
			&& props.getProp("Color", c) 
			&& props.getProp("Font", f))){
				std::cerr << "Error -> Could not parse TEXT object. Make sure you have: \n-\"Position\", \n-\"Text\", \n-\"Font\", & \n-\"Color\" properties.\n";
				break;
			}
			temp->setColor(Hex(c.c_str()));
			temp->draw(renderer, f, t.c_str(), p);
			items.emplace_back(std::move(temp));
			break;
		}
		case objType::IMAGE: {
			FPoint po;
			std::string pa;
			float s;
			if(!(props.getProp("Path", pa) 
			&& props.getProp("Position", po) 
			&& props.getProp("Scale", s))){
				std::cerr << "Error -> Could not parse IMAGE object. Make sure you have \n-\"Path\", \n-\"Position\", & \n-\"Scale\" properties.\n";
				break;
			}
			temp->draw(renderer, pa.c_str());
			temp->setTransform(po);
			temp->getTransform().setScale(s);
			items.emplace_back(std::move(temp));
			break;
		}
		case objType::POLYGON: {
			FPoint p;
			std::vector<int> i;
			std::vector<Vertex> v;
			std::string c;
			if(!(props.getProp("Vertices", v) 
			&& props.getProp("Position", p) 
			&& props.getProp("Indices", i)
			&& props.getProp("Color", c))){
				std::cerr << "Error -> Could not parse POLYGON object. Make sure you have \n-\"Vertices\", \n-\"Position\", \n-\"Color\", & \n-\"Indices\" properties.\n";
				break;
			}
			temp->draw(renderer, i, v);
			temp->setColor(Hex(c.c_str()));
			temp->setTransform(p);
			items.emplace_back(std::move(temp));
			break;
		}
		case objType::BUTTON : {
			FBound b;
			float r;
			std::string cb, ct, t;
			FPoint p;
			Font f;
			if(!(props.getProp("Boundary", b) 
			&& props.getProp("Backgound", cb) 
			&& props.getProp("Roundness", r)
			&& props.getProp("Position", p) 
			&& props.getProp("Text", t) 
			&& props.getProp("Color", ct) 
			&& props.getProp("Font", f))){
				std::cerr << "Error -> Could not parse BUTTON object. Make sure you have \n-\"Boundary\", \n-\"Roundness\", \n-\"Backgound\", \n-\"Position\", \n-\"Text\", \n-\"Font\", & \n-\"Color\" properties.\n";
				break;
			}
			temp->setColor(Hex(cb.c_str()));
			temp->setTransform(b);
			temp->draw(renderer, r);
			items.emplace_back(std::move(temp));
			// TEXT
			auto tx = std::make_unique<zbj>();
			tx->setColor(Hex(ct.c_str()));
			tx->draw(renderer, f, t.c_str(), p);
			items.emplace_back(std::move(tx));
			break;
		}
		case objType::INPUT : {
			FBound b;
			float r;
			std::string cb, ct, t;
			FPoint p;
			Font f;
			if(!(props.getProp("Boundary", b) 
			&& props.getProp("Backgound", cb) 
			&& props.getProp("Roundness", r)
			&& props.getProp("Position", p) 
			&& props.getProp("Text", t) 
			&& props.getProp("Color", ct) 
			&& props.getProp("Font", f))){
				std::cerr << "Error -> Could not parse INPUT object. Make sure you have \n-\"Boundary\", \n-\"Roundness\", \n-\"Backgound\", \n-\"Position\", \n-\"Text\", \n-\"Font\", & \n-\"Color\" properties.\n";
				break;
			}
			temp->setColor(Hex(cb.c_str()));
			temp->setTransform(b);
			temp->draw(renderer, r);
			items.emplace_back(std::move(temp));
			// BORDER (kalau memang beda ya)
			auto bd = std::make_unique<zbj>();
			bd->setColor(Hex(cb.c_str())); // sama warnanya?
			bd->setTransform(b);
			bd->draw(renderer, r, 0); // mode border?
			items.emplace_back(std::move(bd));
			// TEXT
			auto tx = std::make_unique<zbj>();
			tx->setColor(Hex(ct.c_str()));
			tx->draw(renderer, f, t.c_str(), p);
			items.emplace_back(std::move(tx));
			break;
		}
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
	std::cerr << "zbjs::show() with " << items.size() << " items.\n";
	for(auto& i : items){
		if(!i){
			std::cerr << "Error -> Could not show objects, item null.\n";
			return;
		}
		i->show(renderer);
		std::cerr << "Showing item...\n";
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