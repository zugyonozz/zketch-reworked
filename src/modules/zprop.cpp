#include "zprop.h"

objProp::objProp(objType t) : type(t) {
    props = getTemplateForType(t);
}

const std::unordered_map<std::string, Propvals>& objProp::getTemplateForType(objType t) {
	static const std::unordered_map<objType, std::unordered_map<std::string, Propvals>> templates = {
		{objType::RECT, {
			{"Boundary", FBound{0.0f, 0.0f, 100.0f, 100.0f}},
			{"Background", std::string("FFF")},
			{"Roundness", 0.0f},
			{"Border", 0.0f},
			{"Border-Color", std::string("FFF")},
		}},
		{objType::TEXT, {
			{"Text", std::string("Hello")},
			{"Font", nullptr},
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
			{"Background", std::string("FFF")},
			{"Vertices", std::vector<Vertex>{}},
			{"Indices", std::vector<int>{}},
			{"Position", FPoint{}}
		}},
		{objType::TEXTBOX, {
			{"Boundary", FBound{0.0f, 0.0f, 100.0f, 100.0f}},
			{"Roundness", 0.0f},
			{"Background", std::string("FFF")},
			{"Border", 0.0f},
			{"Border-Color", "FFF"},
			{"Text", std::string("Hello")},
			{"Font", nullptr},
			{"Position", FPoint{}},
			{"Color", std::string("F0F0F0")}
		}}
	};

	auto it = templates.find(t);
	if (it != templates.end()) return it->second;
	static const std::unordered_map<std::string, Propvals> empty;
	return empty;
}