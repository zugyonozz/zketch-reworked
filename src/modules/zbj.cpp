#include "zbjs.h"

zbjs::zbjs(Renderer& renderer, objProp& props) : props(props) {
    zbj temp;
    switch (props.type) {
        case objType::LINE: {
            FPoint p1, p2;
            std::string c;
            if (!(props.getProp("xy1", p1) && 
                props.getProp("xy2", p2) && 
                props.getProp("Color", c))) {
                std::cerr << "Error -> Could not parse LINE object. Make sure you have \n-\"xy1\", \n-\"xy2\", & \n-\"Color\" properties.\n";
                break;
            }
            temp.setColor(Hex(c.c_str()));
            temp.draw(renderer, p1, p2);
            items.push_back(new zbj(temp));
            break;
        }
        case objType::RECT: {
            FBound b;
            float r;
            std::string c;
            if (!(props.getProp("Boundary", b) && 
                props.getProp("Color", c) && 
                props.getProp("Roundness", r))) {
                std::cerr << "Error -> Could not parse RECT object. Make sure you have \n-\"Boundary\", \n-\"Roundness\", & \n-\"Color\" properties.\n";
                break;
            }
            temp.setColor(Hex(c.c_str()));
            temp.setTransform(b);
            temp.draw(renderer, r);
            items.push_back(new zbj(temp));
            break;
        }
        case objType::TEXT: {
            FPoint p;
            std::string c, t;
            Font f;
            if (!(props.getProp("Position", p) && 
                props.getProp("Text", t) && 
                props.getProp("Color", c) && 
                props.getProp("Font", f))) {
                std::cerr << "Error -> Could not parse TEXT object. Make sure you have: \n-\"Position\", \n-\"Text\", \n-\"Font\", & \n-\"Color\" properties.\n";
                break;
            }
            temp.setColor(Hex(c.c_str()));
            temp.draw(renderer, f, t.c_str(), p);
            items.push_back(new zbj(temp));
            break;
        }
        case objType::IMAGE: {
            FPoint po;
            std::string pa;
            float s;
            if (!(props.getProp("Path", pa) && 
                props.getProp("Position", po) && 
                props.getProp("Scale", s))) {
                std::cerr << "Error -> Could not parse IMAGE object. Make sure you have \n-\"Path\", \n-\"Position\", & \n-\"Scale\" properties.\n";
                break;
            }
            temp.draw(renderer, pa.c_str());
            temp.setTransform(po);
            temp.getTransform().setScale(s);
            items.push_back(new zbj(temp));
            break;
        }
        case objType::POLYGON: {
            FPoint p;
            std::vector<int> i;
            std::vector<Vertex> v;
            std::string c;
            if (!(props.getProp("Vertices", v) && 
                props.getProp("Position", p) && 
                props.getProp("Indices", i) && 
                props.getProp("Color", c))) {
                std::cerr << "Error -> Could not parse POLYGON object. Make sure you have \n-\"Vertices\", \n-\"Position\", \n-\"Color\", & \n-\"Indices\" properties.\n";
                break;
            }
            temp.draw(renderer, i, v);
            temp.setColor(Hex(c.c_str()));
            temp.setTransform(p);
            items.push_back(new zbj(temp));
            break;
        }
        case objType::BUTTON: {
            FBound b;
            float r;
            std::string cb, ct, t;
            FPoint p;
            Font f;
            if (!(props.getProp("Boundary", b) && 
                props.getProp("Backgound", cb) && 
                props.getProp("Roundness", r) && 
                props.getProp("Position", p) && 
                props.getProp("Text", t) && 
                props.getProp("Color", ct) && 
                props.getProp("Font", f))) {
                std::cerr << "Error -> Could not parse BUTTON object. Make sure you have \n-\"Boundary\", \n-\"Roundness\", \n-\"Backgound\", \n-\"Position\", \n-\"Text\", \n-\"Font\", & \n-\"Color\" properties.\n";
                break;
            }
            temp.setColor(Hex(cb.c_str()));
            temp.setTransform(b);
            temp.draw(renderer, r);
            items.push_back(new zbj(temp));
            
            zbj textObj;
            textObj.setColor(Hex(ct.c_str()));
            textObj.draw(renderer, f, t.c_str(), p);
            items.push_back(new zbj(textObj));
            break;
        }
        case objType::INPUT: {
            FBound b;
            float r;
            std::string cb, ct, t;
            FPoint p;
            Font f;
            if (!(props.getProp("Boundary", b) && 
                props.getProp("Backgound", cb) && 
                props.getProp("Roundness", r) && 
                props.getProp("Position", p) && 
                props.getProp("Text", t) && 
                props.getProp("Color", ct) && 
                props.getProp("Font", f))) {
                std::cerr << "Error -> Could not parse INPUT object. Make sure you have \n-\"Boundary\", \n-\"Roundness\", \n-\"Backgound\", \n-\"Position\", \n-\"Text\", \n-\"Font\", & \n-\"Color\" properties.\n";
                break;
            }
            // Background
            temp.setColor(Hex(cb.c_str()));
            temp.setTransform(b);
            temp.draw(renderer, r);
            items.push_back(new zbj(temp));
            
            // Border
            zbj borderObj;
            borderObj.setColor(Hex(cb.c_str()));
            borderObj.setTransform(b);
            borderObj.draw(renderer, r, false);
            items.push_back(new zbj(borderObj));
            
            // Text
            zbj textObj;
            textObj.setColor(Hex(ct.c_str()));
            textObj.draw(renderer, f, t.c_str(), p);
            items.push_back(new zbj(textObj));
            break;
        }
        default:
            std::cerr << "Error -> Invalid object type. Make sure you have a valid type.\n";
            break;
    }
}

zbjs::~zbjs() {
    for (auto& item : items) {
        if (item) {
            item->clear();
            delete item;
            item = nullptr;
        }
    }
    items.clear();
}

void zbjs::addItem(Renderer& renderer, zbj& item) {
    items.push_back(new zbj(item));
}

void zbjs::insertItem(Renderer& renderer, zbj& item, size_t& id) {
    if (id >= items.size()) {
        items.push_back(new zbj(item));
    } else {
        items.insert(items.begin() + id, new zbj(item));
    }
}

void zbjs::removeItem(size_t& id) {
    if (id >= items.size()) {
        std::cerr << "Error -> Invalid id. Make sure you have a valid id.\n";
        return;
    }
    
    if (items[id]) {
        items[id]->clear();
        delete items[id];
        items[id] = nullptr;
    }
    
    items.erase(items.begin() + id);
}

void zbjs::show(Renderer& renderer) {
    for (auto& item : items) {
        if (item) {
            item->show(renderer);
        }
    }
}

const objProp& zbjs::getProps() const {
    return props;
}

const std::vector<zbj*>& zbjs::getItems() const {
    return items;
}