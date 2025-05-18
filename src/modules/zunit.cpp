#include "zunit.h"

objProp::objProp(objType t) : type(t) {
    props = getTemplateForType(t);
}

const std::unordered_map<std::string, Propvals>& objProp::getTemplateForType(objType t) {
	static const std::unordered_map<objType, std::unordered_map<std::string, Propvals>> templates = {
		{objType::RECT, {
			{"Boundary", FBound{0.0f, 0.0f, 100.0f, 100.0f}},
			{"Color", std::string("FFF")},
			{"Roundness", 0.0f}
		}},
		{objType::TEXT, {
			{"Text", std::string("Hello")},
			{"Font", Font(nullptr)},
			{"Color", std::string("FFF")},
			{"Position", FPoint{}}
		}},
		{objType::IMAGE, {
			{"Path", std::string("default.png")},
			{"Position", FPoint{}},
			{"Scale", 1.0f}
		}},
		{objType::LINE, {
			{"Color", std::string("FFF")},
			{"xy1", FPoint{0.0f, 0.0f}},
			{"xy2", FPoint{0.0f, 0.0f}}
		}},
		{objType::POLYGON, {
			{"Color", std::string("FFF")},
			{"Vertices", std::vector<Vertex>{}},
			{"Indices", std::vector<int>{}},
			{"Position", FPoint{}}
		}},
		{objType::BUTTON, {
			{"Boundary", FBound{0.0f, 0.0f, 100.0f, 100.0f}},
			{"Roundness", 0.0f},
			{"Background", std::string("FFF")},
			{"Text", std::string("Hello")},
			{"Font", Font(nullptr)},
			{"Position", FPoint{}},
			{"Color", std::string("F0F0F0")}
		}},
		{objType::INPUT, {
			{"Boundary", FBound{0.0f, 0.0f, 100.0f, 100.0f}},
			{"Roundness", 0.0f},
			{"Background", std::string("FFF")},
			{"Text", std::string("")},
			{"Font", Font(nullptr)},
			{"Position", FPoint{}},
			{"Color", std::string("FFF")}
		}}
	};

	auto it = templates.find(t);
	if (it != templates.end()) return it->second;
	static const std::unordered_map<std::string, Propvals> empty;
	return empty;
}

template<typename T>
bool objProp::getProp(const std::string& key, T& outVal) const {
	auto it = props.find(key);
	if (it != props.end()) {
		if (auto val = std::get_if<T>(&it->second)) {
			outVal = *val;
			return true;
		}
	}
	return false;
}

void Transform::CalculateTransform(){
	switch(a) {
		case Anchor::ANCHOR_TOP_LEFT:
			p.x = b.x;
			p.y = b.y;
			break;
		case Anchor::ANCHOR_TOP_MID:
			p.x = b.x - b.w / 2;
			p.y = b.y;
			break;
		case Anchor::ANCHOR_TOP_RIGHT:
			p.x = b.x - b.w;
			p.y = b.y;
			break;
		case Anchor::ANCHOR_RIGHT_MID:
			p.x = b.x - b.w;
			p.y = b.y - b.h / 2;
			break;
		case Anchor::ANCHOR_BOT_RIGHT:
			p.x = b.x - b.w;
			p.y = b.y - b.h;
			break;
		case Anchor::ANCHOR_BOT_MID:
			p.x = b.x - b.w / 2;
			p.y = b.y - b.h;
			break;
		case Anchor::ANCHOR_BOT_LEFT:
			p.x = b.x;
			p.y = b.y - b.h;
			break;
		case Anchor::ANCHOR_LEFT_MID:
			p.x = b.x;
			p.y = b.y - b.h / 2;
			break;
		case Anchor::ANCHOR_CENTER:
			p.x = b.x - b.w / 2;
			p.y = b.y - b.h / 2;
			break;
	}
}

Transform::Transform(FBound b) : b(b), p({}), a(Anchor::ANCHOR_CENTER) {}
Transform::Transform(FBound b, Anchor a) : b(b), p({}), a(a) {}

void Transform::setBound(FBound b){
	this->b = b;
}

void Transform::setPosition(FPoint p){
	b.x = p.x;
	b.y = p.y;
}

void Transform::setAnchor(Anchor a){
	this->a = a;
}

void Transform::setScale(float& n){
	b.w *= n;
	b.h *= n;
}

void Transform::setScale(float& nw, float& nh){
	b.w *= nw;
	b.h *= nh;
}

const FPoint& Transform::getPoint() {
	CalculateTransform();
	return p;
}

const FBound& Transform::getBound(bool RealBound) {
	CalculateTransform();
	if(RealBound){
		return b;
	}else{
		FBound newBound = b;
		newBound.x = p.x;
		newBound.y = p.y;
		return newBound;
	}
}

const Anchor& Transform::getAnchor() const{
	return a;
}