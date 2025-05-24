#include "zfuncs.h"
#include <stdexcept>
#include <string>

zFuncs::zFuncs(std::initializer_list<std::function<void()>> funcList) 
    : funcs(funcList) {
    // Direct initialization is more efficient than loop
}

void zFuncs::add(std::function<void()> func) { 
    funcs.emplace_back(std::move(func)); 
}

void zFuncs::operator()() { 
    executeAll();
}

void zFuncs::executeAll() {
    for (auto& f : funcs) { 
        if (f) {  // Check if function is valid before calling
            f(); 
        }
    } 
}

size_t zFuncs::size() const noexcept {
    return funcs.size();
}

bool zFuncs::empty() const noexcept {
    return funcs.empty();
}

void zFuncs::execute(size_t index) {
    if (index >= funcs.size()) {
        throw std::out_of_range("Index " + std::to_string(index) + 
                               " is out of range. Size: " + std::to_string(funcs.size()));
    }
    
    if (funcs[index]) {
        funcs[index]();
    }
}

bool zFuncs::tryExecute(size_t index) noexcept {
    if (index < funcs.size() && funcs[index]) {
        try {
            funcs[index]();
            return true;
        } catch (...) {
            // Swallow exceptions in the safe version
            return false;
        }
    }
    return false;
}

void zFuncs::clear() noexcept {
    funcs.clear();
}

void zFuncs::reserve(size_t capacity) {
    funcs.reserve(capacity);
}

void zFuncs::remove(size_t index) {
    if (index >= funcs.size()) {
        throw std::out_of_range("Index " + std::to_string(index) + 
                               " is out of range. Size: " + std::to_string(funcs.size()));
    }
    
    funcs.erase(funcs.begin() + index);
}

bool zFuncs::isValidIndex(size_t index) const noexcept {
    return index < funcs.size();
}