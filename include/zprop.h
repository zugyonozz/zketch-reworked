#pragma once
#include "zenv.h"
#include <unordered_map>
#include <variant>

enum class objType{
	LINE, RECT, IMAGE, TEXT, POLYGON, BUTTON, INPUT
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