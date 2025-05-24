// zprop.h
#pragma once
#include <variant>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <memory>
#include "ztype.h"
#include "zfont.h"

enum class objType {
    LINE, RECT, IMAGE, TEXT, POLYGON, TEXTBOX
};

// Hash function for objType enum
struct objTypeHash {
    std::size_t operator()(const objType& type) const noexcept {
        return static_cast<std::size_t>(type);
    }
};

using Propvals = std::variant<
    float, 
    std::string, 
    std::vector<int>, 
    std::vector<Vertex>, 
    FBound, 
    FPoint, 
    Font
>;

class objProp {
private:
    objType type_;
    std::unordered_map<std::string, Propvals> props_;
    
    // Static template cache for better memory management
    static const std::map<objType, std::unordered_map<std::string, Propvals>>& getTemplates();

public:
    explicit objProp(objType t);
    
    // Copy constructor and assignment
    objProp(const objProp& other) = default;
    objProp& operator=(const objProp& other) = default;
    
    // Move constructor and assignment
    objProp(objProp&& other) noexcept = default;
    objProp& operator=(objProp&& other) noexcept = default;
    
    // Getters
    objType getType() const noexcept { return type_; }
    const std::unordered_map<std::string, Propvals>& getProps() const noexcept { return props_; }
    
    // Template method for getting properties with type safety
    template<typename T>
    bool getProp(const std::string& key, T& outVal) const;
    
    // Template method for setting properties with type safety
    template<typename T>
    bool setProp(const std::string& key, const T& value);
    
    // Check if property exists
    bool hasProp(const std::string& key) const noexcept;
    
    // Get property names for this object type
    std::vector<std::string> getPropertyNames() const;
    
    // Reset to template defaults
    void resetToDefaults();
    
    static const std::unordered_map<std::string, Propvals>& getTemplateForType(objType t);
};

// Template implementation
template<typename T>
bool objProp::getProp(const std::string& key, T& outVal) const {
    auto it = props_.find(key);
    if (it != props_.end()) {
        if (auto val = std::get_if<T>(&it->second)) {
            outVal = *val;
            return true;
        }
    }
    return false;
}

template<typename T>
bool objProp::setProp(const std::string& key, const T& value) {
    // Check if this property is valid for this object type
    const auto& templateProps = getTemplateForType(type_);
    if (templateProps.find(key) == templateProps.end()) {
        return false; // Property not valid for this object type
    }
    
    // Verify type compatibility
    if (std::holds_alternative<T>(templateProps.at(key))) {
        props_[key] = value;
        return true;
    }
    return false; // Type mismatch
}