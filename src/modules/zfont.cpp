#include "zfont.h"

zfont::zfont() {}

zfont::~zfont() {
    cleanUpFonts();
}

bool zfont::loadFont(const std::string& name, size_t size, const std::string& path) {
    std::string key = name + "_" + std::to_string(size);
    
    // Check if font already loaded
    if (fonts.count(key) > 0 && fonts[key].font != nullptr) {
        return true;
    }
    
    // Attempt to load the font
    TTF_Font* font = TTF_OpenFont(path.c_str(), static_cast<int>(size));
    if (!font) { 
        std::cerr << "Error -> Could not load font: " << path << "\n" << SDL_GetError() << "\n"; 
        return false; 
    }
    
    fonts[key] = { font, size };
    return true;
}

Font zfont::getFont(const std::string& name, size_t size) const {
    std::string key = name + "_" + std::to_string(size);
    auto it = fonts.find(key);
    
    if (it != fonts.end()) { 
        return it->second.font; 
    }
    
    return nullptr;
}

bool zfont::cleanUpFonts() {
    for (auto& [key, data] : fonts) { 
        if (data.font) {
            TTF_CloseFont(data.font);
            data.font = nullptr;
        }
    }
    
    fonts.clear();
    return true;
}