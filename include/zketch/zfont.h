// zfont.h
#pragma once
#include "SDL_ttf.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

using Font = TTF_Font*;

struct FontData {
    Font font;
    size_t size;
    
    FontData() : font(nullptr), size(0) {}
    FontData(Font f, size_t s) : font(f), size(s) {}
    
    // Move constructor and assignment
    FontData(FontData&& other) noexcept;
    
    FontData& operator=(FontData&& other) noexcept;
    
    // Disable copy constructor and assignment to prevent double-free
    FontData(const FontData&) = delete;
    FontData& operator=(const FontData&) = delete;
    
    ~FontData();
};

// Custom hash function for font keys (name + size combination)
struct FontKey {
    std::string name;
    size_t size;
    
    FontKey(const std::string& n, size_t s) : name(n), size(s) {}
    bool operator==(const FontKey& other) const;
};

struct FontKeyHash {
    std::size_t operator()(const FontKey& key) const;
};

using FontMap = std::unordered_map<FontKey, std::unique_ptr<FontData>, FontKeyHash>;

class zfont {
private:
    FontMap fonts_;
    bool sdl_initialized_;
    
    // Helper method to initialize SDL_ttf if needed
    bool ensureSDLInitialized();

public:
    zfont();
    ~zfont();
    
    // Disable copy constructor and assignment
    zfont(const zfont&) = delete;
    zfont& operator=(const zfont&) = delete;
    
    // Enable move constructor and assignment
    zfont(zfont&& other) noexcept;
    zfont& operator=(zfont&& other) noexcept;
    
    // Load a font with specified name, size, and file path
    bool loadFont(const std::string& name, size_t size, const std::string& path);
    
    // Get a font by name and size (returns nullptr if not found)
    Font getFont(const std::string& name, size_t size) const;
    
    // Check if font exists
    bool hasFont(const std::string& name, size_t size) const;
    
    // Remove a specific font
    bool removeFont(const std::string& name, size_t size);
    
    // Get number of loaded fonts
    size_t getFontCount() const noexcept { return fonts_.size(); }
    
    // Clear all fonts
    void clear();
    
    // Get all loaded font keys
    std::vector<std::pair<std::string, size_t>> getLoadedFonts() const;
};