#pragma once
#include <unordered_map>
#include "zenv.h"

struct FontData {
    Font font;
    size_t size;
}; 

using Fonts = std::unordered_map<std::string, FontData>;

class zfont {
private:
    Fonts fonts;

public:
    zfont();
    ~zfont();
    
    bool loadFont(const std::string& name, size_t size, const std::string& path);
    Font getFont(const std::string& name, size_t size) const;
    bool cleanUpFonts();
};