#pragma once
#include "zenv.h"
#include <unordered_map>
#include <variant>

enum class objType{
	LINE, RECT, IMAGE, TEXT, POLYGON, TEXTBOX
};

using Propvals = std::variant<float, std::string, std::vector<int>, std::vector<Vertex>, FBound, FPoint, Font>;

struct objProp {
    objType type;
    std::unordered_map<std::string, Propvals> props;

    objProp(objType t);
    const std::unordered_map<std::string, Propvals>& getTemplateForType(objType t);
	template<typename T>
	bool getProp(const std::string& key, T& outVal) const;
};

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