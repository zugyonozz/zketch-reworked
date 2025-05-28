// zfont.cpp
#include "zfont.h"
#include <utility>

FontData::FontData(FontData&& other) noexcept : font(other.font), size(other.size) {
	other.font = nullptr;
	other.size = 0;
}

FontData& FontData::operator=(FontData&& other) noexcept {
	if (this != &other) {
		if (font) {
			TTF_CloseFont(font);
		}
		font = other.font;
		size = other.size;
		other.font = nullptr;
		other.size = 0;
	}
	return *this;
}

FontData::~FontData() {
	if (font) {
		TTF_CloseFont(font);
	}
}

bool FontKey::operator==(const FontKey& other) const {
	return name == other.name && size == other.size;
}

std::size_t FontKeyHash::operator()(const FontKey& key) const {
	return std::hash<std::string>{}(key.name) ^ (std::hash<size_t>{}(key.size) << 1);
}

zfont::zfont() : sdl_initialized_(false) {
    ensureSDLInitialized();
}

zfont::~zfont() {
    clear();
    if (sdl_initialized_) {
        TTF_Quit();
    }
}

zfont::zfont(zfont&& other) noexcept 
    : fonts_(std::move(other.fonts_)), sdl_initialized_(other.sdl_initialized_) {
    other.sdl_initialized_ = false;
}

zfont& zfont::operator=(zfont&& other) noexcept {
    if (this != &other) {
        clear();
        if (sdl_initialized_) {
            TTF_Quit();
        }
        
        fonts_ = std::move(other.fonts_);
        sdl_initialized_ = other.sdl_initialized_;
        other.sdl_initialized_ = false;
    }
    return *this;
}

bool zfont::ensureSDLInitialized() {
    if (!sdl_initialized_) {
        if (TTF_Init()) {
            sdl_initialized_ = true;
            return true;
        }
		std::cerr << "Failed to initialize SDL_ttf: " << SDL_GetError() << std::endl;
		return false;
    }
    return true;
}

bool zfont::loadFont(const std::string& name, size_t size, const std::string& path) {
    if (!ensureSDLInitialized()) {
        return false;
    }
    
    if (name.empty() || path.empty() || size == 0) {
        return false;
    }
    
    FontKey key(name, size);
    
    // Check if font already exists
    if (fonts_.find(key) != fonts_.end()) {
        return true; // Already loaded
    }
    
    Font font = TTF_OpenFont(path.c_str(), static_cast<int>(size));
    if (!font) {
        std::cerr << "Failed to load font from " << path << ": " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create FontData and store in map
    auto fontData = std::make_unique<FontData>(font, size);
    fonts_[key] = std::move(fontData);
    
    return true;
}

Font zfont::getFont(const std::string& name, size_t size) const {
    FontKey key(name, size);
    auto it = fonts_.find(key);
    
    if (it != fonts_.end() && it->second) {
        return it->second->font;
    }
    
    return nullptr;
}

bool zfont::hasFont(const std::string& name, size_t size) const {
    FontKey key(name, size);
    return fonts_.find(key) != fonts_.end();
}

bool zfont::removeFont(const std::string& name, size_t size) {
    FontKey key(name, size);
    auto it = fonts_.find(key);
    
    if (it != fonts_.end()) {
        fonts_.erase(it);
        return true;
    }
    
    return false;
}

void zfont::clear() {
    fonts_.clear();
}

std::vector<std::pair<std::string, size_t>> zfont::getLoadedFonts() const {
    std::vector<std::pair<std::string, size_t>> result;
    result.reserve(fonts_.size());
    
    for (const auto& pair : fonts_) {
        result.emplace_back(pair.first.name, pair.first.size);
    }
    
    return result;
}