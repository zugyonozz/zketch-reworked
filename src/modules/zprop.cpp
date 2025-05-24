// zprop.cpp
#include "zprop.h"
#include <stdexcept>

objProp::objProp(objType t) : type_(t) {
    props_ = getTemplateForType(t);
}

const std::map<objType, std::unordered_map<std::string, Propvals>>& objProp::getTemplates() {
    static std::map<objType, std::unordered_map<std::string, Propvals>> templates;
    static bool initialized = false;
    
    if (!initialized) {
        // RECT template
        templates[objType::RECT]["Boundary"] = FBound{};
        templates[objType::RECT]["Background"] = std::string("FFFFFF");
        templates[objType::RECT]["Roundness"] = 0.0f;
        templates[objType::RECT]["Border"] = 0.0f;
        templates[objType::RECT]["Border-Color"] = std::string("000000");
        
        // TEXT template
        templates[objType::TEXT]["Text"] = std::string("Hello");
        templates[objType::TEXT]["Font"] = static_cast<Font>(nullptr);
        templates[objType::TEXT]["Color"] = std::string("000000");
        templates[objType::TEXT]["Position"] = FPoint{};
        
        // IMAGE template
        templates[objType::IMAGE]["Path"] = std::string("default.png");
        templates[objType::IMAGE]["Position"] = FPoint{};
        templates[objType::IMAGE]["Scale"] = 1.0f;
        
        // LINE template
        templates[objType::LINE]["Color"] = std::string("000000");
        templates[objType::LINE]["xy1"] = FPoint{};
        templates[objType::LINE]["xy2"] = FPoint{};
        
        // POLYGON template
        templates[objType::POLYGON]["Background"] = std::string("FFFFFF");
        templates[objType::POLYGON]["Vertices"] = std::vector<Vertex>{};
        templates[objType::POLYGON]["Indices"] = std::vector<int>{};
        templates[objType::POLYGON]["Position"] = FPoint{};
        
        // TEXTBOX template
        templates[objType::TEXTBOX]["Boundary"] = FBound{};
        templates[objType::TEXTBOX]["Roundness"] = 0.0f;
        templates[objType::TEXTBOX]["Background"] = std::string("FFFFFF");
        templates[objType::TEXTBOX]["Border"] = 0.0f;
        templates[objType::TEXTBOX]["Border-Color"] = std::string("000000");
        templates[objType::TEXTBOX]["Text"] = std::string("");
        templates[objType::TEXTBOX]["Font"] = static_cast<Font>(nullptr);
        templates[objType::TEXTBOX]["Position"] = FPoint{};
        templates[objType::TEXTBOX]["Color"] = std::string("000000");
        
        initialized = true;
    }
    
    return templates;
}

const std::unordered_map<std::string, Propvals>& objProp::getTemplateForType(objType t) {
    const auto& templates = getTemplates();
    auto it = templates.find(t);
    if (it != templates.end()) {
        return it->second;
    }
    
    static const std::unordered_map<std::string, Propvals> empty;
    return empty;
}

bool objProp::hasProp(const std::string& key) const noexcept {
    return props_.find(key) != props_.end();
}

std::vector<std::string> objProp::getPropertyNames() const {
    std::vector<std::string> names;
    names.reserve(props_.size());
    
    for (const auto& pair : props_) {
        names.push_back(pair.first);
    }
    
    return names;
}

void objProp::resetToDefaults() {
    props_ = getTemplateForType(type_);
}