// utils.cpp
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hexCharToInt(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1; // Karakter tidak valid
}

Color Hex(const char* hex) {
    SDL_Color color = {0, 0, 0, 255};
    if (hex == nullptr) return color;
    if (hex[0] == '#') hex++;

    size_t len = strlen(hex);
    if (len == 3) {
        // Format RGB
        color.r = hexCharToInt(hex[0]) * 17; // Duplikasi digit untuk mendapatkan nilai 0-255
        color.g = hexCharToInt(hex[1]) * 17;
        color.b = hexCharToInt(hex[2]) * 17;
    } else if (len == 4) {
        // Format RGBA
        color.r = hexCharToInt(hex[0]) * 17;
        color.g = hexCharToInt(hex[1]) * 17;
        color.b = hexCharToInt(hex[2]) * 17;
        color.a = hexCharToInt(hex[3]) * 17;
    } else if (len == 6) {
        // Format RRGGBB
        color.r = (hexCharToInt(hex[0]) << 4) + hexCharToInt(hex[1]);
        color.g = (hexCharToInt(hex[2]) << 4) + hexCharToInt(hex[3]);
        color.b = (hexCharToInt(hex[4]) << 4) + hexCharToInt(hex[5]);
    } else if (len == 8) {
        // Format RRGGBBAA
        color.r = (hexCharToInt(hex[0]) << 4) + hexCharToInt(hex[1]);
        color.g = (hexCharToInt(hex[2]) << 4) + hexCharToInt(hex[3]);
        color.b = (hexCharToInt(hex[4]) << 4) + hexCharToInt(hex[5]);
        color.a = (hexCharToInt(hex[6]) << 4) + hexCharToInt(hex[7]);
    }
    return color;
}

// Konversi Point ke FPoint
FPoint Point2FPoint(Point& p) {
	return FPoint{
		(float)p.x, 
		(float)p.y
	};
}

// Konversi FPoint ke Point
Point FPoint2Point(FPoint& p) {
	return Point{
		(int)p.x, 
		(int)p.y
	};
}

// Konversi Bound ke FBound
FBound Bound2FBound(Bound& b) {
	return FBound{
		(float)b.x, 
		(float)b.y, 
		(float)b.w, 
		(float)b.h
	};
}

// Konversi Bound ke FBound
Bound FBound2Bound(FBound& b) {
	return Bound{
		(int)b.x, 
		(int)b.y, 
		(int)b.w, 
		(int)b.h
	};
}

// Konversi Color ke FColor
FColor Color2FColor(Color& c){
	return { 
		(float)c.r / 255.0f, 
		(float)c.g / 255.0f, 
		(float)c.b / 255.0f, 
		(float)c.a / 255.0f 
	}; 
}

// Konversi FColor ke Color
Color FColor2Color(FColor& fc) {
    return {
        (Uint8)(fc.r * 255.0f),
        (Uint8)(fc.g * 255.0f),
        (Uint8)(fc.b * 255.0f),
        (Uint8)(fc.a * 255.0f)
    };
}

FPoint getMousePos(){
	FPoint coord;
	SDL_PumpEvents();
	SDL_GetMouseState(&coord.x, &coord.y);
	return coord;
}

bool isHover(Bound b){
	FPoint coord = getMousePos();
	if(coord.x >= b.x && coord.x <= b.x + b.w && coord.y >= b.y && coord.y <= b.y + b.h){
		return true;
	}
	return false;
}

bool isOnClick(const Event& e, Bound b) {
    int mx = e.button.x;
    int my = e.button.y;
    return (mx >= b.x && mx <= b.x + b.w && my >= b.y && my <= b.y + b.h);
}
